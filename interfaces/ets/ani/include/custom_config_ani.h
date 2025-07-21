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

#ifndef CUSTOM_CONFIG_ANI_H
#define CUSTOM_CONFIG_ANI_H

#include <string>

#include <ani.h>

namespace OHOS {
namespace Customization {
namespace ConfigPolicy {
class CustomConfigAni {
public:
    CustomConfigAni();
    ~CustomConfigAni() = default;
    static ani_status Init(ani_env* vm);

private:
    static ani_string GetChannelId(ani_env* env);
    static int GetBundleName(std::string &bundleName);
    static bool IsInPreloadList(std::string bundleName);
    static char* CustGetSystemParam(const char *name);
};
} // namespace ConfigPolicy
} // namespace Customization
} // namespace OHOS
#endif
