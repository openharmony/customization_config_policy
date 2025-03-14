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

#include "custom_config_napi.h"

#include "application_context.h"
#include "hilog/log.h"
#include "hisysevent_adapter.h"
#include "init_param.h"

#undef LOG_DOMAIN
#define LOG_DOMAIN 0xD001E00

#undef LOG_TAG
#define LOG_TAG "CustomConfigJs"

namespace OHOS {
namespace Customization {
namespace ConfigPolicy {
using namespace OHOS::HiviewDFX;

static const std::string CHANNEL_ID_PREFIX = "const.channelid.";
static const std::string CUSTOM_PRELOAD_LIST_PARA = "persist.custom.preload.list";

napi_value CustomConfigNapi::Init(napi_env env, napi_value exports)
{
    napi_property_descriptor property[] = {
        DECLARE_NAPI_FUNCTION("getChannelId", CustomConfigNapi::NAPIGetChannelId),
    };
    NAPI_CALL(env, napi_define_properties(env, exports, sizeof(property) / sizeof(property[0]), property));
    return exports;
}

int CustomConfigNapi::GetBundleName(std::string &bundleName)
{
    std::shared_ptr<AbilityRuntime::ApplicationContext> abilityContext =
        AbilityRuntime::Context::GetApplicationContext();
    if (abilityContext == nullptr) {
        HILOG_ERROR(LOG_CORE, "get abilityContext failed.");
        return -1;
    }
    bundleName = abilityContext->GetBundleName();
    return 0;
}

 bool CustomConfigNapi::IsInPreloadList(std::string bundleName)
 {
    char *preloadList = CustGetSystemParam(CUSTOM_PRELOAD_LIST_PARA.c_str());
    if (preloadList == nullptr) {
        HILOG_WARN(LOG_CORE, "get preload list fail.");
        return false;
    }
    if (preloadList[0] < '0' || preloadList[0] > '9' || preloadList[1] != ',') {
        HILOG_ERROR(LOG_CORE, "preload list param format error.");
        free(preloadList);
        return false;
    }
    int listlen = preloadList[0] - '0';
    std::string preloadListResult(preloadList + 1); // skip listlen
    free(preloadList);

    for (int i = 1; i < listlen; i++) {
        std::string tempPreloadListPara = CUSTOM_PRELOAD_LIST_PARA + std::to_string(i);
        char *tempList = CustGetSystemParam(tempPreloadListPara.c_str());
        if (tempList == nullptr) {
            HILOG_ERROR(LOG_CORE, "preload list len error.");
            return false;
        }
        preloadListResult.append(tempList + 1); // skip listlen
        free(tempList);
    }
    preloadListResult.append(",");
    return preloadListResult.find("," + bundleName + ",") != std::string::npos;
}

napi_value CustomConfigNapi::NAPIGetChannelId(napi_env env, napi_callback_info info)
{
    std::string bundleName;
    if (GetBundleName(bundleName) != 0 || bundleName.empty() || IsInPreloadList(bundleName)) {
        return CreateNapiStringValue(env, "");
    }
    std::string channelKey = CHANNEL_ID_PREFIX + bundleName;
    return NativeGetChannelId(env, channelKey);
}

char *CustomConfigNapi::CustGetSystemParam(const char *name)
{
    char *value = nullptr;
    unsigned int len = 0;

    if (SystemGetParameter(name, nullptr, &len) != 0 || len <= 0 || len > PARAM_CONST_VALUE_LEN_MAX) {
        return nullptr;
    }
    value = (char *)calloc(len, sizeof(char));
    if (value != nullptr && SystemGetParameter(name, value, &len) == 0 && value[0]) {
        return value;
    }
    if (value != nullptr) {
        free(value);
    }
    return nullptr;
}

napi_value CustomConfigNapi::NativeGetChannelId(napi_env env, std::string channelKey)
{
    char *channelId = CustGetSystemParam(channelKey.c_str());
    napi_value result = nullptr;
    if (channelId == nullptr) {
        HILOG_WARN(LOG_CORE, "get channelId failed.");
        ReportConfigPolicyEvent(ReportType::CONFIG_POLICY_FAILED, "getChannelId", "ChannelId is nullptr.");
        return CreateNapiStringValue(env, "");
    }
    result = CreateNapiStringValue(env, channelId);
    free(channelId);
    return result;
}

napi_value CustomConfigNapi::CreateNapiStringValue(napi_env env, const char *str)
{
    napi_value result = nullptr;
    NAPI_CALL(env, napi_create_string_utf8(env, str, NAPI_AUTO_LENGTH, &result));
    return result;
}

static napi_value CustomConfigInit(napi_env env, napi_value exports)
{
    return CustomConfigNapi::Init(env, exports);
}

static napi_module g_customConfigModule = {
    .nm_version = 1,
    .nm_flags = 0,
    .nm_filename = nullptr,
    .nm_register_func = CustomConfigInit,
    .nm_modname = "customConfig",
    .nm_priv = ((void *)0),
    .reserved = { 0 }
};

extern "C" __attribute__((constructor)) void CustomConfigRegister()
{
    napi_module_register(&g_customConfigModule);
}
} // namespace ConfigPolicy
} // namespace Customization
} // namespace OHOS
