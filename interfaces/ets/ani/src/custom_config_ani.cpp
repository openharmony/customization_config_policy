/*
 * Copyright (c) 2025 Huawei Device Co., Ltd.
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

#include "custom_config_ani.h"

#include "application_context.h"
#include "hilog/log.h"
#include "hisysevent_adapter.h"
#include "init_param.h"
#include "ani_utils.h"

#undef LOG_DOMAIN
#define LOG_DOMAIN 0xD001E00

#undef LOG_TAG
#define LOG_TAG "CustomConfigJs"

namespace OHOS {
namespace Customization {
namespace ConfigPolicy {
using namespace OHOS::HiviewDFX;

static const char* NAMESPACE_NAME = "@ohos.customization.customConfig.customConfig";
static const std::string CHANNEL_ID_PREFIX = "const.channelid.";
static const std::string CUSTOM_PRELOAD_LIST_PARA = "persist.custom.preload.list";

ani_status CustomConfigAni::Init(ani_env* env)
{
    ani_namespace ns;
    if (ANI_OK != env->FindNamespace(NAMESPACE_NAME, &ns)) {
        HILOG_ERROR(LOG_CORE, "Not found namespace %{public}s.", NAMESPACE_NAME);
        return ANI_ERROR;
    }
    std::array methods = {
        ani_native_function { "getChannelId", nullptr, reinterpret_cast<void*>(GetChannelId) },
    };
    if (ANI_OK != env->Namespace_BindNativeFunctions(ns, methods.data(), methods.size())) {
        HILOG_ERROR(LOG_CORE, "Cannot bind native methods to namespace[%{public}s]", NAMESPACE_NAME);
        return ANI_ERROR;
    };
    return ANI_OK;
}

int CustomConfigAni::GetBundleName(std::string &bundleName)
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

bool CustomConfigAni::IsInPreloadList(std::string bundleName)
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

char* CustomConfigAni::CustGetSystemParam(const char *name)
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

ani_string CustomConfigAni::GetChannelId(ani_env* env)
{
    std::string bundleName;
    if (GetBundleName(bundleName) != 0 || bundleName.empty() || IsInPreloadList(bundleName)) {
        return AniUtils::StringToAniStr(env, "");
    }
    std::string channelKey = CHANNEL_ID_PREFIX + bundleName;
    char *channelId = CustGetSystemParam(channelKey.c_str());
    if (channelId == nullptr) {
        HILOG_WARN(LOG_CORE, "get channelId failed.");
        ReportConfigPolicyEvent(ReportType::CONFIG_POLICY_FAILED, "getChannelId", "ChannelId is nullptr.");
        return AniUtils::StringToAniStr(env, "");
    }
    std::string channelIdStr(channelId);
    ani_string result = AniUtils::StringToAniStr(env, channelIdStr);
    free(channelId);
    return result;
}
} // namespace ConfigPolicy
} // namespace Customization
} // namespace OHOS

ANI_EXPORT ani_status ANI_Constructor(ani_vm* vm, uint32_t* result)
{
    ani_env* env;
    if (ANI_OK != vm->GetEnv(ANI_VERSION_1, &env)) {
        HILOG_ERROR(LOG_CORE, "Unsupported ANI_VERSION_1.");
        return (ani_status)ANI_ERROR;
    }

    auto status = OHOS::Customization::ConfigPolicy::CustomConfigAni::Init(env);
    if (status != ANI_OK) {
        return status;
    }
    *result = ANI_VERSION_1;
    return ANI_OK;
}
