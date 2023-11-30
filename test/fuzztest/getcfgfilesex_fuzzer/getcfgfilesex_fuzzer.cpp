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

#define FUZZ_PROJECT_NAME "getcfgfilesex_fuzzer"

constexpr size_t MIN_SIZE = 4;

namespace OHOS {
    bool FuzzGetCfgFilesEx(const uint8_t* data, size_t size)
    {
        std::string cfgPath(reinterpret_cast<const char*>(data), size / 2);
        std::string extra(reinterpret_cast<const char*>(data) + size / 2, size - size / 2);
        int followMode = (data[0] << 24) | (data[1] << 16) | (data[2] << 8) | data[3];
        CfgFiles *cfgFiles = GetCfgFilesEx(cfgPath.c_str(), followMode, extra.c_str());
        bool result = cfgFiles != nullptr;
        FreeCfgFiles(cfgFiles);
        return result;
    }
}

// Fuzzer entry point.
extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
    if (data == nullptr || size < MIN_SIZE) {
        return 0;
    }
    // Run your code on data.
    OHOS::FuzzGetCfgFilesEx(data, size);
    return 0;
}