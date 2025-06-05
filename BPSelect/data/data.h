#pragma once
#include "hero.h"
#include <string>

// 从JSON文件加载英雄数据
bool loadHeroData(const string& filename);

// 将Role字符串转换为枚举
Role stringToRole(const string& roleStr);

// 将Position字符串转换为枚举
Position stringToPosition(const string& posStr); 