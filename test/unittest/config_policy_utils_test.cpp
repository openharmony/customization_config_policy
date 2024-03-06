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
#include "config_policy_impl.h"
#ifndef __LITEOS__
#include "init_param.h"
#include "telephony_config_c.h"
#endif

using namespace testing::ext;

namespace OHOS {
class ConfigPolicyUtilsTest : public testing::Test {
    public:
        static void SetUpTestCase(void);
};

#ifndef __LITEOS__
void ConfigPolicyUtilsTest::SetUpTestCase(void)
{
    SystemSetParameter(CUST_OPKEY0, "46060");
    SystemSetParameter(CUST_OPKEY1, "46061");
    SystemSetParameter(CUST_FOLLOW_X_RULES,
        ":etc/custxmltest/user.xml,10:etc/custxmltest/both.xml,100,etc/carrier/${test:-46061}");
}
#endif

bool TestGetCfgFile(const char *testPathSuffix, int type, const char *extra)
{
    CfgFiles *cfgFiles = GetCfgFilesEx(testPathSuffix, type, extra);
    EXPECT_TRUE(cfgFiles != NULL);
    bool flag = false;
    char *filePath = nullptr;
    for (size_t i = 0; i < MAX_CFG_POLICY_DIRS_CNT; i++) {
        filePath = cfgFiles->paths[i];
        if (filePath && *filePath != '\0') {
            std::cout << "type: " << type << ", filePath: " << filePath << std::endl;
            flag = true;
        }
    }
    FreeCfgFiles(cfgFiles);
    char buf[MAX_PATH_LEN] = {0};
    filePath = GetOneCfgFileEx(testPathSuffix, buf, MAX_PATH_LEN, type, extra);
    if (filePath && *filePath != '\0') {
        std::cout << "type: " << type << ", one filePath: " << filePath << std::endl;
        flag = flag && true;
    }
    return flag;
}

/**
 * @tc.name: CfgPolicyUtilsFuncTest001
 * @tc.desc: Test GetOneCfgFile & GetCfgFiles function, none file case.
 * @tc.type: FUNC
 * @tc.require: issueI5NYDH
 */
HWTEST_F(ConfigPolicyUtilsTest, CfgPolicyUtilsFuncTest001, TestSize.Level1)
{
    const char *testPathSuffix = "etc/custxmltest/none.xml";
    EXPECT_FALSE(TestGetCfgFile(testPathSuffix, FOLLOWX_MODE_NO_RULE_FOLLOWED, NULL));
}

/**
 * @tc.name: CfgPolicyUtilsFuncTest002
 * @tc.desc: Test GetOneCfgFile & GetCfgFiles function, system file case.
 * @tc.type: FUNC
 * @tc.require: issueI5NYDH
 */
HWTEST_F(ConfigPolicyUtilsTest, CfgPolicyUtilsFuncTest002, TestSize.Level1)
{
    const char *testPathSuffix = "etc/custxmltest/system.xml";
    EXPECT_TRUE(TestGetCfgFile(testPathSuffix, FOLLOWX_MODE_NO_RULE_FOLLOWED, NULL));
}

/**
 * @tc.name: CfgPolicyUtilsFuncTest003
 * @tc.desc: Test GetOneCfgFile & GetCfgFiles function, user file case.
 * @tc.type: FUNC
 * @tc.require: issueI5NYDH
 */
HWTEST_F(ConfigPolicyUtilsTest, CfgPolicyUtilsFuncTest003, TestSize.Level1)
{
    const char *testPathSuffix = "etc/custxmltest/user.xml";
    EXPECT_TRUE(TestGetCfgFile(testPathSuffix, FOLLOWX_MODE_NO_RULE_FOLLOWED, NULL));
}

/**
 * @tc.name: CfgPolicyUtilsFuncTest004
 * @tc.desc: Test GetOneCfgFile & GetCfgFiles function, both files case.
 * @tc.type: FUNC
 * @tc.require: issueI5NYDH
 */
HWTEST_F(ConfigPolicyUtilsTest, CfgPolicyUtilsFuncTest004, TestSize.Level1)
{
    const char *testPathSuffix = "etc/custxmltest/both.xml";
    EXPECT_TRUE(TestGetCfgFile(testPathSuffix, FOLLOWX_MODE_NO_RULE_FOLLOWED, NULL));
}

/**
 * @tc.name: CfgPolicyUtilsFuncTest005
 * @tc.desc: Test struct CfgDir *GetCfgDirList(void) function.
 * @tc.type: FUNC
 * @tc.require: issueI5NYDH
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

#ifndef __LITEOS__
/**
 * @tc.name: CfgPolicyUtilsFuncTest006
 * @tc.desc: Test GetOneCfgFile & GetCfgFiles function, follow default sim rule.
 * @tc.type: FUNC
 * @tc.require: issueI5NYDH
 */
HWTEST_F(ConfigPolicyUtilsTest, CfgPolicyUtilsFuncTest006, TestSize.Level1)
{
    const char *testPathSuffix = "etc/custxmltest/user.xml";
    EXPECT_TRUE(TestGetCfgFile(testPathSuffix, FOLLOWX_MODE_SIM_DEFAULT, NULL));
}

/**
 * @tc.name: CfgPolicyUtilsFuncTest007
 * @tc.desc: Test GetOneCfgFile & GetCfgFiles function, follow sim1 rule.
 * @tc.type: FUNC
 * @tc.require: issueI5NYDH
 */
HWTEST_F(ConfigPolicyUtilsTest, CfgPolicyUtilsFuncTest007, TestSize.Level1)
{
    const char *testPathSuffix = "etc/custxmltest/user.xml";
    EXPECT_TRUE(TestGetCfgFile(testPathSuffix, FOLLOWX_MODE_SIM_1, NULL));
}

/**
 * @tc.name: CfgPolicyUtilsFuncTest008
 * @tc.desc: Test GetOneCfgFile & GetCfgFiles function, follow sim1 rule.
 * @tc.type: FUNC
 * @tc.require: issueI5NYDH
 */
HWTEST_F(ConfigPolicyUtilsTest, CfgPolicyUtilsFuncTest008, TestSize.Level1)
{
    const char *testPathSuffix = "etc/custxmltest/user.xml";
    EXPECT_TRUE(TestGetCfgFile(testPathSuffix, FOLLOWX_MODE_SIM_2, NULL));
}

/**
 * @tc.name: CfgPolicyUtilsFuncTest009
 * @tc.desc: Test GetOneCfgFile & GetCfgFiles function, follow default x rule rule.
 * @tc.type: FUNC
 * @tc.require: issueI5NYDH
 */
HWTEST_F(ConfigPolicyUtilsTest, CfgPolicyUtilsFuncTest009, TestSize.Level1)
{
    EXPECT_TRUE(TestGetCfgFile("etc/custxmltest/user.xml", FOLLOWX_MODE_DEFAULT, NULL));
    EXPECT_TRUE(TestGetCfgFile("etc/custxmltest/both.xml", FOLLOWX_MODE_DEFAULT, NULL));
}

/**
 * @tc.name: CfgPolicyUtilsFuncTest010
 * @tc.desc: Test GetOneCfgFile & GetCfgFiles function, follow default x rule rule.
 * @tc.type: FUNC
 * @tc.require: issueI5NYDH
 */
HWTEST_F(ConfigPolicyUtilsTest, CfgPolicyUtilsFuncTest010, TestSize.Level1)
{
    std::string extraString;
    extraString.append("etc/carrier/${").append(CUST_OPKEY1).append(":-46060},etc/carrier/${")
        .append(CUST_OPKEY0).append("}/etc/${testkey:-custxmltest}");
    std::cout << "extra: " << extraString << std::endl;
    EXPECT_TRUE(TestGetCfgFile("etc/custxmltest/user.xml", FOLLOWX_MODE_USER_DEFINED, extraString.c_str()));
    EXPECT_TRUE(TestGetCfgFile("etc/custxmltest/both.xml", FOLLOWX_MODE_USER_DEFINED, extraString.c_str()));
}
#endif // __LITEOS__
} // namespace OHOS
