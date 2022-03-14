# Config Policy

## Introduction

The config policy, namely, cust, provides APIs for each service module to obtain the configuration directories at different levels or the configuration file paths.

## Directory Structure

The directory structure for the config policy is as follows:

```
/base/customization/
├── config_policy             # Code repository for the config policy
│   ├── frameworks            # Core code of the config policy
│   │   ├── config_policy     # config policy
│   │   │   └── src           # Implementation code
│   ├── interfaces            # APIs of the config policy
│   │   ├── innerkits         # APIs of the config policy for internal subsystems
│   │   └── kits              # JavaScript APIs
│   └── test                  # Test code
```

## Usage

Call the APIs of the config policy to obtain the configuration directories at different levels or the configuration file paths.

```
#include "config_policy_utils.h"

const char *testPathSuffix = "user.xml"; // Set the name of the configuration file.
char buf[MAX_PATH_LEN];
char *filePath = GetOneCfgFile(testPathSuffix, CUST_TYPE_CONFIG, buf, MAX_PATH_LEN); // Obtain the path of the configuration file with the highest priority.
```

**Programming language**: C/C++
