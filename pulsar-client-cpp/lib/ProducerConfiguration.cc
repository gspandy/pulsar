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

#include <lib/ProducerConfigurationImpl.h>


namespace pulsar {
ProducerConfiguration::ProducerConfiguration()
        : impl_(boost::make_shared<ProducerConfigurationImpl>()) {
}

ProducerConfiguration::~ProducerConfiguration() {
}

ProducerConfiguration::ProducerConfiguration(const ProducerConfiguration& x)
        : impl_(x.impl_) {
}

ProducerConfiguration& ProducerConfiguration::operator=(const ProducerConfiguration& x) {
    impl_ = x.impl_;
    return *this;
}

ProducerConfiguration& ProducerConfiguration::setSendTimeout(int sendTimeoutMs) {
    impl_->sendTimeoutMs = sendTimeoutMs;
    return *this;
}

int ProducerConfiguration::getSendTimeout() const {
    return impl_->sendTimeoutMs;
}

ProducerConfiguration& ProducerConfiguration::setCompressionType(CompressionType compressionType) {
    impl_->compressionType = compressionType;
    return *this;
}

CompressionType ProducerConfiguration::getCompressionType() const {
    return impl_->compressionType;
}

ProducerConfiguration& ProducerConfiguration::setMaxPendingMessages(int maxPendingMessages) {
    assert(maxPendingMessages > 0);
    impl_->maxPendingMessages = maxPendingMessages;
    return *this;
}

int ProducerConfiguration::getMaxPendingMessages() const {
    return impl_->maxPendingMessages;
}

ProducerConfiguration& ProducerConfiguration::setPartitionsRoutingMode(
        const PartitionsRoutingMode& mode) {
    impl_->routingMode = mode;
    return *this;
}

ProducerConfiguration::PartitionsRoutingMode ProducerConfiguration::getPartitionsRoutingMode() const {
    return impl_->routingMode;
}

ProducerConfiguration& ProducerConfiguration::setMessageRouter(
        const MessageRoutingPolicyPtr& router) {
    impl_->routingMode = ProducerConfiguration::CustomPartition;
    impl_->messageRouter = router;
    return *this;
}

const MessageRoutingPolicyPtr& ProducerConfiguration::getMessageRouterPtr() const {
    return impl_->messageRouter;
}

ProducerConfiguration& ProducerConfiguration::setBlockIfQueueFull(bool flag) {
    impl_->blockIfQueueFull = flag;
    return *this;
}

bool ProducerConfiguration::getBlockIfQueueFull() const {
    return impl_->blockIfQueueFull;
}

ProducerConfiguration& ProducerConfiguration::setBatchingEnabled(const bool& batchingEnabled) {
    impl_->batchingEnabled = batchingEnabled;
    return *this;
}
const bool& ProducerConfiguration::getBatchingEnabled() const {
    return impl_->batchingEnabled;
}

ProducerConfiguration& ProducerConfiguration::setBatchingMaxMessages(
        const unsigned int& batchingMaxMessages) {
    assert(batchingMaxMessages > 1);
    impl_->batchingMaxMessages = batchingMaxMessages;
    return *this;
}

const unsigned int& ProducerConfiguration::getBatchingMaxMessages() const {
    return impl_->batchingMaxMessages;
}

ProducerConfiguration& ProducerConfiguration::setBatchingMaxAllowedSizeInBytes(
        const unsigned long& batchingMaxAllowedSizeInBytes) {
    impl_->batchingMaxAllowedSizeInBytes = batchingMaxAllowedSizeInBytes;
    return *this;
}
const unsigned long& ProducerConfiguration::getBatchingMaxAllowedSizeInBytes() const {
    return impl_->batchingMaxAllowedSizeInBytes;
}

ProducerConfiguration& ProducerConfiguration::setBatchingMaxPublishDelayMs(
        const unsigned long& batchingMaxPublishDelayMs) {
    impl_->batchingMaxPublishDelayMs = batchingMaxPublishDelayMs;
    return *this;
}

const unsigned long& ProducerConfiguration::getBatchingMaxPublishDelayMs() const {
    return impl_->batchingMaxPublishDelayMs;
}
}
