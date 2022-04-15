/*
 * Copyright (c) 2021-2022 Huawei Device Co., Ltd.
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

#include "config_policy_utils_test.h"

#include <gtest/gtest.h>

#include "config_policy_utils.h"

using namespace testing::ext;

namespace OHOS {
class ConfigPolicyUtilsTest : public testing::Test {};

bool TestGetCfgFile(const char *testPathSuffix)
{
    CfgFiles *cfgFiles = GetCfgFiles(testPathSuffix);
    EXPECT_TRUE(cfgFiles != NULL);
    bool flag = false;
    char *filePath = nullptr;
    for (size_t i = 0; i < MAX_CFG_POLICY_DIRS_CNT; i++) {
        filePath = cfgFiles->paths[i];
        if (filePath && *filePath != '\0') {
            std::cout << "filePath: " << filePath << std::endl;
            flag = true;
        }
    }
    FreeCfgFiles(cfgFiles);
    char buf[MAX_PATH_LEN] = {0};
    filePath = GetOneCfgFile(testPathSuffix, buf, MAX_PATH_LEN);
    if (filePath && *filePath != '\0') {
        std::cout << "one filePath: " << filePath << std::endl;
        flag = flag && true;
    }
    return flag;
}

/**
 * @tc.name: CfgPolicyUtilsFuncTest001
 * @tc.desc: Test GetOneCfgFile & GetCfgFiles function, none file case.
 * @tc.type: FUNC
 */
HWTEST_F(ConfigPolicyUtilsTest, CfgPolicyUtilsFuncTest001, TestSize.Level1)
{
    const char *testPathSuffix = "custxmltest/none.xml";
    EXPECT_FALSE(TestGetCfgFile(testPathSuffix));
}

/**
 * @tc.name: CfgPolicyUtilsFuncTest002
 * @tc.desc: Test GetOneCfgFile & GetCfgFiles function, system file case.
 * @tc.type: FUNC
 */
HWTEST_F(ConfigPolicyUtilsTest, CfgPolicyUtilsFuncTest002, TestSize.Level1)
{
    const char *testPathSuffix = "custxmltest/system.xml";
    EXPECT_TRUE(TestGetCfgFile(testPathSuffix));
}

/**
 * @tc.name: CfgPolicyUtilsFuncTest003
 * @tc.desc: Test GetOneCfgFile & GetCfgFiles function, user file case.
 * @tc.type: FUNC
 */
HWTEST_F(ConfigPolicyUtilsTest, CfgPolicyUtilsFuncTest003, TestSize.Level1)
{
    const char *testPathSuffix = "custxmltest/user.xml";
    EXPECT_TRUE(TestGetCfgFile(testPathSuffix));
}

/**
 * @tc.name: CfgPolicyUtilsFuncTest004
 * @tc.desc: Test GetOneCfgFile & GetCfgFiles function, both files case.
 * @tc.type: FUNC
 */
HWTEST_F(ConfigPolicyUtilsTest, CfgPolicyUtilsFuncTest004, TestSize.Level1)
{
    const char *testPathSuffix = "custxmltest/both.xml";
    EXPECT_TRUE(TestGetCfgFile(testPathSuffix));
}

/**
 * @tc.name: CfgPolicyUtilsFuncTest005
 * @tc.desc: Test struct CfgDir *GetCfgDirList(void) function.
 * @tc.type: FUNC
 */
HWTEST_F(ConfigPolicyUtilsTest, CfgPolicyUtilsFuncTest005, TestSize.Level1)
{
    CfgDir *cfgDir = GetCfgDirList();
    EXPECT_TRUE(cfgDir != NULL);
    bool flag = false;
    for (size_t i = 0; i < MAX_CFG_POLICY_DIRS_CNT; i++) {
        char *filePath = cfgDir->paths[i];
        if (filePath && *filePath != '\0') {
            std::cout << "filePath: " << filePath << std::endl;
            flag = true;
        }
    }
    FreeCfgDirList(cfgDir);
    EXPECT_TRUE(flag);
}
} // namespace OHOS
