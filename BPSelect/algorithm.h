#pragma once
#include "hero.h"
#include <algorithm>
#include <queue>
#include <vector>
#include <unordered_map>
using namespace std;

// 检查英雄是否已被选择或禁用
bool isAlreadyPickedOrBanned(int id);

//从优先队列中获取前N个元素
vector<int> topN(priority_queue<pair<float,int>>& pq, int n);

// 建议禁用英雄
vector<int> suggestBan(int num=3);

// 建议选择英雄
vector<int> suggestPick();

// 计算英雄强度
float calculateTier(const Hero& hero);

// 检查位置冲突
bool hasPositionConflict(int heroId, const vector<int>& team);

// 获取英雄主要位置
Position getPrimaryPosition(int heroId);

// 获取团队位置分布
unordered_map<Position, int> getTeamPositionDistribution(const vector<int>& team);

// 获取团队角色分布
unordered_map<Role, int> getTeamRoleDistribution(const vector<int>& team);

// 评估阵容平衡性
float evaluateTeamBalance(const vector<int>& team);

// 评估阵容强度
float evaluateTeamStrength(const vector<int>& team);

// 评估阵容协同性
float evaluateTeamSynergy(const vector<int>& team);

// 计算直接克制关系
float calculateDirectCounter(int heroId, int targetId);

// 计算间接克制关系
float calculateIndirectCounter(int heroId, int targetId);

// 计算角色克制关系
float calculateRoleCounter(int heroId, int targetId);

// 计算位置克制关系
float calculatePositionCounter(int heroId, int targetId);

// 计算综合克制关系
float calculateTotalCounter(int heroId, const vector<int>& targets);

// 计算直接协同关系
float calculateDirectSynergy(int heroId, int allyId);

// 计算间接协同关系
float calculateIndirectSynergy(int heroId, int allyId);

// 计算角色协同关系
float calculateRoleSynergy(int heroId, int allyId);

// 计算位置协同关系
float calculatePositionSynergy(int heroId, int allyId);

// 计算技能协同关系
float calculateSkillSynergy(int heroId, int allyId);

// 计算综合协同关系
float calculateTotalSynergy(int heroId, const vector<int>& allies);

// 计算阵容评分
struct TeamScore {
    float balance;      // 平衡性评分
    float strength;     // 强度评分
    float synergy;      // 协同性评分
    float counter;      // 克制评分
    float total;        // 总分
};

// 计算多维度阵容评分
TeamScore calculateTeamScore(const vector<int>& team, const vector<int>& enemies);




