/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
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

#ifndef CONFIG_POLICY_FFI_H
#define CONFIG_POLICY_FFI_H

#include <cstdint>

#define FFI_EXPORT __attribute__((visibility("default")))

extern "C" {
    const int32_t SUCCESS_CODE = 0;
    struct CArrString {
        char** head;
        int64_t size;
    };

    struct RetDataCArrString {
        int32_t code;
        CArrString data;
    };

    struct RetDataCString {
        int32_t code;
        char* data;
    };
    
    FFI_EXPORT RetDataCArrString CJ_GetCfgDirList();
    FFI_EXPORT RetDataCArrString CJ_GetCfgFiles(const char* relPath);
    FFI_EXPORT RetDataCString CJ_GetOneCfgFile(const char* relPath);
}

#endif