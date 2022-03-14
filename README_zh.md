# 定制框架组件介绍

## 简介

定制框架组件为各业务模块提供获取各配置层级的配置目录或配置文件路径的接口。

## 目录

定制框架组件源代码目录结构如下所示：

```
/base/customization/
├── config_policy           # 定制框架代码仓
│   ├── frameworks          # 定制框架核心代码
│   │   ├── config_policy   # 定制框架模块
│   │   │   └── src         # 实现代码
│   ├── interfaces          # 定制框架接口
│   │   ├── innerkits       # 定制框架子系统间接口
│   │   └── kits            # 定制框架JavaScript接口
│   └── test                # 测试代码
```

## 说明

调用该组件中的接口获取各配置层级的配置目录或配置文件路径。

```
#include "config_policy_utils.h"

const char *testPathSuffix = "user.xml"; //设置配置文件名称
char buf[MAX_PATH_LEN];
char *filePath = GetOneCfgFile(testPathSuffix, CUST_TYPE_CONFIG, buf, MAX_PATH_LEN); //获取最高优先级的配置文件路径
```

**语言限制**：C/C++语言
