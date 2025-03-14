/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef CUSTOM_CONFIG_NAPI_H
#define CUSTOM_CONFIG_NAPI_H

#include <string>

#include "napi/native_common.h"
#include "napi/native_node_api.h"
#include "napi/native_api.h"

namespace OHOS {
namespace Customization {
namespace ConfigPolicy {
class CustomConfigNapi {
public:
    CustomConfigNapi();
    ~CustomConfigNapi() = default;
    static napi_value Init(napi_env env, napi_value exports);

private:
    static int GetBundleName(std::string &bundleName);
    static bool IsInPreloadList(std::string bundleName);
    static napi_value NAPIGetChannelId(napi_env env, napi_callback_info info);
    static napi_value NativeGetChannelId(napi_env env, std::string channelKey);
    static napi_value CreateNapiStringValue(napi_env env, const char *str);
    static char *CustGetSystemParam(const char *name);
};
} // namespace ConfigPolicy
} // namespace Customization
} // namespace OHOS
#endif
