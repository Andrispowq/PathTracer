#pragma once

#include <iostream>

#include <algorithm>

#include <map>
#include <set>
#include <unordered_map>
#include <vector>
#include <array>
#include <memory>
#include <string>
#include <sstream>
#include <functional>

#include <chrono>
#include <thread>
#include <fstream>
#include <optional>

#include "engine/core/util/log/Log.h"
#include "engine/core/util/time/Time.h"

#include "platform/Platform.h"

#include "engine/core/util/codes/InputCode.h"

#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

#define BIT(x) (0x1 << x)