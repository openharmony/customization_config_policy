/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
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

#include <cstddef>
#include <cstdint>
#include <string>

#include "config_policy_utils.h"

#define FUZZ_PROJECT_NAME "getonecfgfile_fuzzer"


namespace OHOS {
    bool FuzzGetOneCfgFile(const uint8_t* data, size_t size)
    {
        std::string userPath(reinterpret_cast<const char*>(data), size);
        char buf[MAX_PATH_LEN] = {0};
        char *filePath = GetOneCfgFile(userPath.c_str(), buf, MAX_PATH_LEN);
        return filePath != nullptr;
    }
}

// Fuzzer entry point.
extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
    if (data == nullptr) {
        return 0;
    }
    // Run your code on data.
    OHOS::FuzzGetOneCfgFile(data, size);
    return 0;
}