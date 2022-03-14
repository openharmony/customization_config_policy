# Config Policy

## Introduction

The customization framework, namely, cust, provides APIs for each service module to obtain the configuration directories at different levels or the configuration file paths.

## Directory Structure

The directory structure for the customization framework is as follows:

```
/base/customization/
├── config_policy             # Code repository for the customization framework
│   ├── frameworks            # Core code of the customization framework
│   │   ├── config_policy     # Customization framework
│   │   │   └── src           # Implementation code
│   ├── interfaces            # APIs of the customization framework
│   │   ├── innerkits         # APIs of the customization framework for internal subsystems
│   │   └── kits              # JavaScript APIs
│   └── test                  # Test code
```

## Usage

Call the APIs of the customization framework to obtain the configuration directories at different levels or the configuration file paths.

```
#include "config_policy_utils.h"

const char *testPathSuffix = "user.xml"; // Set the name of the configuration file.
char buf[MAX_PATH_LEN];
char *filePath = GetOneCfgFile(testPathSuffix, CUST_TYPE_CONFIG, buf, MAX_PATH_LEN); // Obtain the path of the configuration file with the highest priority.
```

**Programming language**: C/C++
