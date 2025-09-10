#pragma once
#include "hero.h"
#include "algorithm.h"
#include <iomanip>
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// 菜单显示
void displayMenu();

// 英雄列表显示
void displayHeroList();

// 英雄详情显示
void displayHeroDetails(int id);

// 选择建议显示
void showPickSuggestions();

// 当前状态显示
void showCurrentStatus();

// 禁用建议显示
void showBanSuggestions();

// 英雄选择界面显示
void displayHeroSelection(const string& prompt);

// 英雄添加结果显示
void displayHeroAddResult(const Hero& hero, const string& action);

// 团队分析显示
void displayTeamAnalysis();

// 搜索界面显示
void displaySearchInterface();

// 导出选项显示
void displayExportOptions();

// 英雄显示
void displayHero(const Hero& hero); 