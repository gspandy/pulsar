/**
 * Copyright 2016 Yahoo Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
package com.yahoo.pulsar.broker.service.persistent;

import java.util.concurrent.atomic.AtomicIntegerFieldUpdater;

import org.apache.bookkeeper.mledger.AsyncCallbacks.FindEntryCallback;
import org.apache.bookkeeper.mledger.AsyncCallbacks.MarkDeleteCallback;
import org.apache.bookkeeper.mledger.ManagedCursor;
import org.apache.bookkeeper.mledger.ManagedLedgerException;
import org.apache.bookkeeper.mledger.Position;
import org.apache.bookkeeper.mledger.util.Rate;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.yahoo.pulsar.client.impl.MessageImpl;

/**
 */
public class PersistentMessageExpiryMonitor implements FindEntryCallback {
    private final ManagedCursor cursor;
    private final String subName;
    private final String topicName;
    private final Rate msgExpired;

    private static final int FALSE = 0;
    private static final int TRUE = 1;
    @SuppressWarnings("unused")
    private volatile int expirationCheckInProgress = FALSE;
    private static final AtomicIntegerFieldUpdater<PersistentMessageExpiryMonitor> expirationCheckInProgressUpdater = AtomicIntegerFieldUpdater
            .newUpdater(PersistentMessageExpiryMonitor.class, "expirationCheckInProgress");

    public PersistentMessageExpiryMonitor(String topicName, String subscriptionName, ManagedCursor cursor) {
        this.topicName = topicName;
        this.cursor = cursor;
        this.subName = subscriptionName;
        this.msgExpired = new Rate();
    }

    public void expireMessages(int messageTTLInSeconds) {
        if (expirationCheckInProgressUpdater.compareAndSet(this, FALSE, TRUE)) {
            log.info("[{}][{}] Starting message expiry check, ttl= {} seconds", topicName, subName,
                    messageTTLInSeconds);

            cursor.asyncFindNewestMatching(ManagedCursor.FindPositionConstraint.SearchActiveEntries, entry -> {
                MessageImpl msg = null;
                try {
                    msg = MessageImpl.deserialize(entry.getDataBuffer());
                    return msg.isExpired(messageTTLInSeconds);
                } catch (Exception e) {
                    log.error("[{}][{}] Error deserializing message for expiry check", topicName, subName, e);
                } finally {
                    entry.release();
                    if (msg != null) {
                        msg.recycle();
                    }
                }
                return false;
            }, this, null);
        } else {
            if (log.isDebugEnabled()) {
                log.debug("[{}][{}] Ignore expire-message scheduled task, last check is still running", topicName,
                        subName);
            }
        }
    }

    public void updateRates() {
        msgExpired.calculateRate();
    }

    public double getMessageExpiryRate() {
        return msgExpired.getRate();
    }

    private static final Logger log = LoggerFactory.getLogger(PersistentMessageExpiryMonitor.class);

    private final MarkDeleteCallback markDeleteCallback = new MarkDeleteCallback() {
        @Override
        public void markDeleteComplete(Object ctx) {
            long numMessagesExpired = (long) ctx - cursor.getNumberOfEntriesInBacklog();
            msgExpired.recordMultipleEvents(numMessagesExpired, 0 /* no value stats */);
            updateRates();

            if (log.isDebugEnabled()) {
                log.debug("[{}][{}] Mark deleted {} messages", topicName, subName, numMessagesExpired);
            }
        }

        @Override
        public void markDeleteFailed(ManagedLedgerException exception, Object ctx) {
            log.warn("[{}][{}] Message expiry failed - mark delete failed", topicName, subName, exception);
            updateRates();
        }
    };

    @Override
    public void findEntryComplete(Position position, Object ctx) {
        if (position != null) {
            log.info("[{}][{}] Expiring all messages until position {}", topicName, subName, position);
            cursor.asyncMarkDelete(position, markDeleteCallback, cursor.getNumberOfEntriesInBacklog());
        } else {
            if (log.isDebugEnabled()) {
                log.debug("[{}][{}] No messages to expire", topicName, subName);
            }
            updateRates();
        }
        expirationCheckInProgress = FALSE;
    }

    @Override
    public void findEntryFailed(ManagedLedgerException exception, Object ctx) {
        if (log.isDebugEnabled()) {
            log.debug("[{}][{}] Finding expired entry operation failed", topicName, subName, exception);
        }
        expirationCheckInProgress = FALSE;
        updateRates();
    }
}
