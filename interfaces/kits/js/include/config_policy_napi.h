/*
 * Copyright (c) 2022-2023 Huawei Device Co., Ltd.
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

#ifndef CONFIG_POLICY_NAPI_H
#define CONFIG_POLICY_NAPI_H

#include <string>

#include "napi/native_common.h"
#include "napi/native_node_api.h"
#include "napi/native_api.h"

namespace OHOS {
namespace Customization {
namespace ConfigPolicy {
struct ConfigAsyncContext {
    napi_async_work work_;
    napi_deferred deferred_;
    napi_ref callbackRef_;
    typedef napi_value (*CreateNapiValue)(napi_env env, ConfigAsyncContext &context);
    CreateNapiValue createValueFunc_;

    std::string relPath_;
    int32_t followMode_;
    std::string extra_;
    std::string pathValue_;
    std::vector<std::string> paths_;
};

class ConfigPolicyNapi {
public:
    ConfigPolicyNapi();
    ~ConfigPolicyNapi() = default;
    static napi_value Init(napi_env env, napi_value exports);

private:
    typedef napi_value (*NapiFunction)(napi_env env, std::shared_ptr<ConfigAsyncContext> context);

    static napi_value NAPIGetOneCfgFile(napi_env env, napi_callback_info info);
    static napi_value NAPIGetCfgFiles(napi_env env, napi_callback_info info);
    static napi_value NAPIGetOneCfgFileSync(napi_env env, napi_callback_info info);
    static napi_value NAPIGetCfgFilesSync(napi_env env, napi_callback_info info);
    static napi_value NAPIGetCfgDirList(napi_env env, napi_callback_info info);
    static napi_value NAPIGetCfgDirListSync(napi_env env, napi_callback_info info);
    static napi_value CreateUndefined(napi_env env);
    static std::string GetStringFromNAPI(napi_env env, napi_value value);
    static napi_value HandleAsyncWork(napi_env env, ConfigAsyncContext *context, std::string workName,
        napi_async_execute_callback execute, napi_async_complete_callback complete);
    static napi_value GetOneCfgFileOrAllCfgFilesSync(napi_env env, napi_callback_info info, NapiFunction func);
    static napi_value GetOneCfgFileOrAllCfgFiles(napi_env env, napi_callback_info info,
        const std::string &workName, napi_async_execute_callback execute);
    static bool MatchValueType(napi_env env, napi_value value, napi_valuetype targetType);
    static void NativeGetOneCfgFile(napi_env env, void *data);
    static void NativeGetCfgFiles(napi_env env, void *data);
    static void NativeGetCfgDirList(napi_env env, void *data);
    static napi_value NativeGetOneCfgFileSync(napi_env env, std::shared_ptr<ConfigAsyncContext> context);
    static napi_value NativeGetCfgFilesSync(napi_env env, std::shared_ptr<ConfigAsyncContext> context);
    static napi_value NativeGetCfgDirListSync(napi_env env, std::shared_ptr<ConfigAsyncContext> context);
    static void NativeCallbackComplete(napi_env env, napi_status status, void *data);
    static napi_value ParseRelPath(napi_env env, std::string &param, napi_value args);
    static napi_value ParseFollowMode(napi_env env, int32_t &param, napi_value args, bool hasExtra);
    static napi_value ParseExtra(napi_env env, std::string &param, napi_value args);
    static napi_value CreateArraysValue(napi_env env, std::shared_ptr<ConfigAsyncContext> context);
    static void CreateArraysValueFunc(ConfigAsyncContext &context);
    static void CreateFollowXModeObject(napi_env env, napi_value value);
    static napi_value ThrowNapiError(napi_env env, int32_t errCode, const std::string &errMessage);
};
} // namespace ConfigPolicy
} // namespace Customization
} // namespace OHOS
#endif
