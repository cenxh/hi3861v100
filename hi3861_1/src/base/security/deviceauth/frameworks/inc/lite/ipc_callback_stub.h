/*
 * Copyright (C) 2021 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef IPC_CALLBACK_STUB_H
#define IPC_CALLBACK_STUB_H

#include <stdbool.h>
#include <serializer.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    SvcIdentity stubIdentity;
    bool registed;
} StubDevAuthCb;

int32_t CbStubOnRemoteRequest(const IpcContext *ctx, void *ipcMsg, IpcIo *data, void *arg);

#ifdef __cplusplus
}
#endif
#endif