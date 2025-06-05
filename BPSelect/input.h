#pragma once
#include "hero.h"
#include "display.h"
#include <iostream>
#include <string>

using namespace std;

// 添加禁用英雄
void addBan();

// 添加我方选择
void addAllyPick();

// 添加敌方选择
void addEnemyPick();

// 重置选择
void resetSelections();

// 处理英雄选择
void handleHeroSelection(int& id, const string& action);

// 处理继续选择
void handleContinueSelection(const string& action);

// 处理英雄搜索
void handleHeroSearch();

// 处理英雄详情查看
void handleHeroDetails();

// 处理团队分析
void handleTeamAnalysis();

// 处理选择历史
void handleSelectionHistory();

// 处理数据导出
void handleDataExport();

// 保存选择历史
void saveSelectionHistory();

// 加载选择历史
void loadSelectionHistory(); 