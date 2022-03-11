/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
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

#include "config_policy_napi.h"

#include <vector>

#include "hilog/log.h"
#include "config_policy_utils.h"

namespace OHOS {
namespace Customization {
namespace ConfigPolicy {
using namespace OHOS::HiviewDFX;

static constexpr size_t ARGS_SIZE_ONE = 1;
static constexpr size_t ARGS_SIZE_TWO = 2;
static constexpr int32_t ARR_INDEX_ZERO = 0;
static constexpr int32_t ARR_INDEX_ONE = 1;
static constexpr int32_t NAPI_RETURN_ZERO = 0;
static constexpr int32_t NAPI_RETURN_ONE = 1;
static constexpr HiLogLabel LABEL = { LOG_CORE, 0xD001E00, "ConfigPolicyJs" };

napi_value ConfigPolicyNapi::Init(napi_env env, napi_value exports)
{
    napi_property_descriptor property[] = {
        DECLARE_NAPI_FUNCTION("getOneCfgFile", ConfigPolicyNapi::NAPIGetOneCfgFile),
        DECLARE_NAPI_FUNCTION("getCfgFiles", ConfigPolicyNapi::NAPIGetCfgFiles),
        DECLARE_NAPI_FUNCTION("getCfgDirList", ConfigPolicyNapi::NAPIGetCfgDirList)
    };
    NAPI_CALL(env, napi_define_properties(env, exports, sizeof(property) / sizeof(property[0]), property));
    return exports;
}

napi_value ConfigPolicyNapi::NAPIGetOneCfgFile(napi_env env, napi_callback_info info)
{
    size_t argc = ARGS_SIZE_TWO;
    napi_value argv[ARGS_SIZE_TWO] = {nullptr};
    napi_value thisVar = nullptr;
    void *data = nullptr;
    napi_get_cb_info(env, info, &argc, argv, &thisVar, &data);
    NAPI_ASSERT(env, argc >= ARGS_SIZE_ONE, "parameter count error");

    auto asyncContext = (std::make_unique<ConfigAsyncContext>()).release();
    ParseRelPath(env, asyncContext->relPath_, argv[ARR_INDEX_ZERO]);
    if (argc == ARGS_SIZE_TWO) {
        bool matchFlag = MatchValueType(env, argv[ARR_INDEX_ONE], napi_function);
        NAPI_ASSERT(env, matchFlag, "parameter type error");
        napi_create_reference(env, argv[ARR_INDEX_ONE], NAPI_RETURN_ONE, &asyncContext->callbackRef_);
    }
    return HandleAsyncWork(env, asyncContext, "NAPIGetOneCfgFile", NativeGetOneCfgFile, NativeCallbackComplete);
}

napi_value ConfigPolicyNapi::NAPIGetCfgFiles(napi_env env, napi_callback_info info)
{
    size_t argc = ARGS_SIZE_TWO;
    napi_value argv[ARGS_SIZE_TWO] = {nullptr};
    napi_value thisVar = nullptr;
    void *data = nullptr;
    napi_get_cb_info(env, info, &argc, argv, &thisVar, &data);
    NAPI_ASSERT(env, argc >= ARGS_SIZE_ONE, "parameter count error");

    auto asyncContext = (std::make_unique<ConfigAsyncContext>()).release();
    ParseRelPath(env, asyncContext->relPath_, argv[ARR_INDEX_ZERO]);
    if (argc == ARGS_SIZE_TWO) {
        bool matchFlag = MatchValueType(env, argv[ARR_INDEX_ONE], napi_function);
        NAPI_ASSERT(env, matchFlag, "parameter type error");
        napi_create_reference(env, argv[ARR_INDEX_ONE], NAPI_RETURN_ONE, &asyncContext->callbackRef_);
    }
    return HandleAsyncWork(env, asyncContext, "NAPIGetCfgFiles", NativeGetCfgFiles, NativeCallbackComplete);
}

napi_value ConfigPolicyNapi::NAPIGetCfgDirList(napi_env env, napi_callback_info info)
{
    size_t argc = ARGS_SIZE_ONE;
    napi_value argv[ARGS_SIZE_ONE] = {nullptr};
    napi_value thisVar = nullptr;
    void *data = nullptr;
    napi_get_cb_info(env, info, &argc, argv, &thisVar, &data);

    auto asyncContext = (std::make_unique<ConfigAsyncContext>()).release();
    if (argc == ARGS_SIZE_ONE) {
        bool matchFlag = MatchValueType(env, argv[ARR_INDEX_ZERO], napi_function);
        NAPI_ASSERT(env, matchFlag, "parameter type error");
        napi_create_reference(env, argv[ARR_INDEX_ZERO], NAPI_RETURN_ONE, &asyncContext->callbackRef_);
    }
    return HandleAsyncWork(env, asyncContext, "NAPIGetCfgDirList", NativeGetCfgDirList, NativeCallbackComplete);
}

napi_value ConfigPolicyNapi::CreateUndefined(napi_env env)
{
    napi_value result = nullptr;
    napi_get_undefined(env, &result);
    return result;
}

std::string ConfigPolicyNapi::GetStringFromNAPI(napi_env env, napi_value value)
{
    std::string result;
    size_t size = 0;

    if (napi_get_value_string_utf8(env, value, nullptr, NAPI_RETURN_ZERO, &size) != napi_ok) {
        HiLog::Error(LABEL, "can not get string size.");
        return "";
    }
    result.reserve(size + NAPI_RETURN_ONE);
    result.resize(size);
    if (napi_get_value_string_utf8(env, value, result.data(), (size + NAPI_RETURN_ONE), &size) != napi_ok) {
        HiLog::Error(LABEL, "can not get string value.");
        return "";
    }
    return result;
}

napi_value ConfigPolicyNapi::HandleAsyncWork(napi_env env, ConfigAsyncContext *context, std::string workName,
    napi_async_execute_callback execute, napi_async_complete_callback complete)
{
    napi_value result = nullptr;
    if (context->callbackRef_ == nullptr) {
        napi_create_promise(env, &context->deferred_, &result);
    } else {
        napi_get_undefined(env, &result);
    }
    napi_value resource = CreateUndefined(env);
    napi_value resourceName = nullptr;
    napi_create_string_utf8(env, workName.data(), NAPI_AUTO_LENGTH, &resourceName);
    napi_create_async_work(env, resource, resourceName, execute, complete, (void *)context, &context->work_);
    napi_queue_async_work(env, context->work_);
    return result;
}

bool ConfigPolicyNapi::MatchValueType(napi_env env, napi_value value, napi_valuetype targetType)
{
    napi_valuetype valueType = napi_undefined;
    napi_typeof(env, value, &valueType);
    return valueType == targetType;
}

void ConfigPolicyNapi::NativeGetOneCfgFile(napi_env env, void *data)
{
    if (data == nullptr) {
        HiLog::Error(LABEL, "data is nullptr");
        return;
    }
    ConfigAsyncContext *asyncCallbackInfo = (ConfigAsyncContext *)data;
    char outBuf[MAX_PATH_LEN];
    GetOneCfgFile(asyncCallbackInfo->relPath_.c_str(), asyncCallbackInfo->custType_, outBuf, MAX_PATH_LEN);
    asyncCallbackInfo->pathValue_ = std::string(outBuf);
    asyncCallbackInfo->createValueFunc_ = [](napi_env env, ConfigAsyncContext &context) -> napi_value {
        napi_value result;
        napi_status status = napi_create_string_utf8(env, context.pathValue_.c_str(), NAPI_AUTO_LENGTH, &result);
        if (status != napi_ok) {
            context.SetErrorMsg("Failed to get one file.");
            return nullptr;
        }
        return result;
    };
}

void ConfigPolicyNapi::NativeGetCfgFiles(napi_env env, void *data)
{
    if (data == nullptr) {
        HiLog::Error(LABEL, "data is nullptr");
        return;
    }

    ConfigAsyncContext *asyncCallbackInfo = (ConfigAsyncContext *)data;
    CfgFiles *cfgFiles = GetCfgFiles(asyncCallbackInfo->relPath_.c_str(), asyncCallbackInfo->custType_);
    for (size_t i = 0; i < MAX_CFG_POLICY_DIRS_CNT; i++) {
        if (cfgFiles != nullptr && cfgFiles->paths[i] != nullptr) {
            asyncCallbackInfo->paths_.push_back(cfgFiles->paths[i]);
        }
    }
    FreeCfgFiles(cfgFiles);
    asyncCallbackInfo->createValueFunc_ = [](napi_env env, ConfigAsyncContext &context) -> napi_value {
        napi_value result = nullptr;
        napi_status status = napi_create_array_with_length(env, context.paths_.size(), &result);
        if (status != napi_ok) {
            context.SetErrorMsg("Failed to get cfg files.");
            return nullptr;
        }
        for (size_t i = 0; i < context.paths_.size(); i++) {
            napi_value element = nullptr;
            status = napi_create_string_utf8(env, context.paths_[i].c_str(), NAPI_AUTO_LENGTH, &element);
            if (status != napi_ok) {
                context.SetErrorMsg("Failed to create string item.");
                return nullptr;
            }
            status = napi_set_element(env, result, i, element);
            if (status != napi_ok) {
                context.SetErrorMsg("Failed to set array item.");
                return nullptr;
            }
        }
        return result;
    };
}

void ConfigPolicyNapi::NativeGetCfgDirList(napi_env env, void *data)
{
    if (data == nullptr) {
        HiLog::Error(LABEL, "data is nullptr.");
        return;
    }

    ConfigAsyncContext *asyncCallbackInfo = (ConfigAsyncContext *)data;
    CfgDir *cfgDir = GetCfgDirListType(asyncCallbackInfo->custType_);
    for (size_t i = 0; i < MAX_CFG_POLICY_DIRS_CNT; i++) {
        if (cfgDir != nullptr && cfgDir->paths[i] != nullptr) {
            asyncCallbackInfo->paths_.push_back(cfgDir->paths[i]);
        }
    }
    FreeCfgDirList(cfgDir);
    asyncCallbackInfo->createValueFunc_ = [](napi_env env, ConfigAsyncContext &context) -> napi_value {
        napi_value result = nullptr;
        napi_status status = napi_create_array_with_length(env, context.paths_.size(), &result);
        if (status != napi_ok) {
            context.SetErrorMsg("Failed to get file dir.");
            return nullptr;
        }
        for (size_t i = 0; i < context.paths_.size(); i++) {
            napi_value element = nullptr;
            status = napi_create_string_utf8(env, context.paths_[i].c_str(), NAPI_AUTO_LENGTH, &element);
            if (status != napi_ok) {
                context.SetErrorMsg("Failed to create string item.");
                return nullptr;
            }
            status = napi_set_element(env, result, i, element);
            if (status != napi_ok) {
                context.SetErrorMsg("Failed to set array item.");
                return nullptr;
            }
        }
        return result;
    };
}

void ConfigPolicyNapi::NativeCallbackComplete(napi_env env, napi_status status, void *data)
{
    if (data == nullptr) {
        HiLog::Error(LABEL, "data is nullptr");
        return;
    }

    napi_value finalResult = nullptr;
    ConfigAsyncContext *asyncContext = (ConfigAsyncContext *)data;
    if (asyncContext->createValueFunc_ != nullptr) {
        finalResult = asyncContext->createValueFunc_(env, *asyncContext);
    }

    napi_value result[] = { nullptr, nullptr };
    if (asyncContext->success_) {
        napi_get_undefined(env, &result[0]);
        result[1] = finalResult;
    } else {
        napi_value message = nullptr;
        napi_create_string_utf8(env, asyncContext->errMsg_.c_str(), NAPI_AUTO_LENGTH, &message);
        napi_create_error(env, nullptr, message, &result[0]);
        napi_get_undefined(env, &result[1]);
    }

    if (asyncContext->deferred_ != nullptr) {
        if (asyncContext->success_) {
            napi_resolve_deferred(env, asyncContext->deferred_, result[1]);
        } else {
            napi_reject_deferred(env, asyncContext->deferred_, result[0]);
        }
    } else {
        napi_value callback = nullptr;
        napi_value userRet = nullptr;
        napi_get_reference_value(env, asyncContext->callbackRef_, &callback);
        napi_call_function(env, nullptr, callback, sizeof(result) / sizeof(napi_value), result, &userRet);
        napi_delete_reference(env, asyncContext->callbackRef_);
    }
    napi_delete_async_work(env, asyncContext->work_);
    delete asyncContext;
}

napi_value ConfigPolicyNapi::ParseRelPath(napi_env env, std::string &param, napi_value args)
{
    bool matchFlag = MatchValueType(env, args, napi_string);
    NAPI_ASSERT(env, matchFlag, "Wrong argument type, string expected.");
    param = GetStringFromNAPI(env, args);
    napi_value result = nullptr;
    napi_status status = napi_create_int32(env, NAPI_RETURN_ONE, &result);
    NAPI_ASSERT(env, status == napi_ok, "napi_create_int32 error!");
    return result;
}

void ConfigAsyncContext::SetErrorMsg(const std::string &msg)
{
    errMsg_ = msg;
    success_ = false;
    HiLog::Error(LABEL, "%{public}s", msg.c_str());
}

static napi_value ConfigPolicyInit(napi_env env, napi_value exports)
{
    return ConfigPolicyNapi::Init(env, exports);
}

static napi_module g_configPolicyModule = {
    .nm_version = 1,
    .nm_flags = 0,
    .nm_filename = nullptr,
    .nm_register_func = ConfigPolicyInit,
    .nm_modname = "configPolicy",
    .nm_priv = ((void *)0),
    .reserved = { 0 }
};

extern "C" __attribute__((constructor)) void ConfigPolicyRegister()
{
    napi_module_register(&g_configPolicyModule);
}
} // namespace ConfigPolicy
} // namespace Customization
} // namespace OHOS
