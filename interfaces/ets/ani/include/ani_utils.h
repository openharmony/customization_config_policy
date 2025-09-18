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

#ifndef ANI_UTILS_H
#define ANI_UTILS_H

#include <ani.h>
#include <string>

namespace OHOS {
namespace Customization {
namespace ConfigPolicy {
class AniUtils {
public:
    static bool AniStrToString(ani_env *env, ani_string ani_str, std::string& out);
    static ani_string StringToAniStr(ani_env *env, const std::string &str);
    static ani_ref CreateAniStringArray(ani_env *env, const std::vector<std::string> &paths);
    static void ThrowAniError(ani_env *env, int32_t code, const std::string &message);
};
} // namespace ConfigPolicy
} // namespace Customization
} // namespace OHOS
#endif
