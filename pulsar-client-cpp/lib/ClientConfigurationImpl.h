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

#ifndef LIB_CLIENTCONFIGURATIONIMPL_H_
#define LIB_CLIENTCONFIGURATIONIMPL_H_

#include <pulsar/ClientConfiguration.h>

namespace pulsar {

struct ClientConfigurationImpl {
    AuthenticationPtr authenticationPtr;
    int ioThreads;
    int operationTimeoutSeconds;
    int messageListenerThreads;
    int concurrentLookupRequest;
    std::string logConfFilePath;
    bool useTls;
    std::string tlsTrustCertsFilePath;
    bool tlsAllowInsecureConnection;
    ClientConfigurationImpl() : authenticationPtr(AuthFactory::Disabled()),
             ioThreads(1),
             operationTimeoutSeconds(30),
             messageListenerThreads(1),
             concurrentLookupRequest(5000),
             logConfFilePath(),
             useTls(false),
             tlsAllowInsecureConnection(true) {}
};
}

#endif /* LIB_CLIENTCONFIGURATIONIMPL_H_ */
