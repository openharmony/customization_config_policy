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

#ifndef CONFIG_POLICY_ANI_H
#define CONFIG_POLICY_ANI_H

#include <ani.h>
#include <string>

namespace OHOS {
namespace Customization {
namespace ConfigPolicy {

struct ConfigSyncContext {
    std::string relPath;
    int32_t followMode;
    std::string extra;
};

class ConfigPolicyAni {
public:
    ConfigPolicyAni();
    ~ConfigPolicyAni() = default;
    static ani_status Init(ani_env* vm);
private:

    static ani_string GetOneCfgFileSync(ani_env* env, ani_string relPath, ani_enum_item followMode, ani_string extra);

    static ani_ref GetCfgFilesSync(ani_env* env, ani_string relPath, ani_enum_item followMode, ani_string extra);

    static ani_ref GetCfgDirListSync(ani_env* env);

    static bool TransformParams(
        ani_env* env, ani_string relPath, ani_enum_item followMode, ani_string extra, ConfigSyncContext& context);

    static bool ParseFollowMode(
        ani_env* env, ani_enum_item followMode, ConfigSyncContext& context, bool hasExtra);

    static bool ParseExtra(ani_env* env, ani_string extra, ConfigSyncContext& context);
};
} // namespace ConfigPolicy
} // namespace Customization
} // namespace OHOS
#endif
