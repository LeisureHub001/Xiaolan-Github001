#include "algorithm.h"


bool isAlreadyPickedOrBanned(int id) {
    return (find(banned.begin(), banned.end(), id) != banned.end()) ||
           (find(allyPicks.begin(), allyPicks.end(), id) != allyPicks.end()) ||
           (find(enemyPicks.begin(), enemyPicks.end(), id) != enemyPicks.end());
}

vector<int> topN(priority_queue<pair<float,int>>& pq, int n) {
    vector<int> result;
    while (!pq.empty() && n > 0) {
        result.push_back(pq.top().second);
        pq.pop();
        n--;
    }
    return result;
}

// 建议禁用英雄
vector<int> suggestBan(int num) {
    priority_queue<pair<float,int>> scores;
    
    for (auto& [id, hero] : heroDB) {
        // 敌方可能一抢的英雄
        float threat = hero.tier * 0.7; 
        
        // 克制我方已选英雄
        for (int ally : allyPicks) {
            if (find(heroDB[ally].counters.begin(), 
                    heroDB[ally].counters.end(), id) 
                != heroDB[ally].counters.end()) {
                threat += 0.3;
            }
        }
        
        scores.push({threat, id});
    }
    
    // 取威胁度最高的num个
    vector<int> res;
    while(num-- && !scores.empty()){
        res.push_back(scores.top().second);
        scores.pop();
    }
    return res;
}

// 获取英雄主要位置
Position getPrimaryPosition(int heroId) {
    const Hero& hero = heroDB[heroId];
    if (hero.positions.empty()) {
        return Position::SUPPORT; // 默认位置
    }
    return hero.positions[0]; // 返回第一个位置作为主要位置
}

// 获取团队位置分布
unordered_map<Position, int> getTeamPositionDistribution(const vector<int>& team) {
    unordered_map<Position, int> distribution;
    
    // 初始化所有位置计数为0
    for (int i = 0; i < 5; ++i) {
        distribution[static_cast<Position>(i)] = 0;
    }
    
    // 统计每个位置的数量
    for (int id : team) {
        Position primaryPos = getPrimaryPosition(id);
        distribution[primaryPos]++;
    }
    
    return distribution;
}

// 检查位置冲突
bool hasPositionConflict(int heroId, const vector<int>& team) {
    const Hero& hero = heroDB[heroId];
    auto distribution = getTeamPositionDistribution(team);
    
    // 检查新英雄的每个可能位置
    for (Position pos : hero.positions) {
        // 如果该位置已经有英雄，且不是SUPPORT位置（因为可以有多个辅助）
        if (distribution[pos] > 0 && pos != Position::SUPPORT) {
            return true;
        }
    }
    
    // 检查团队是否已经有5个英雄（不考虑SUPPORT位置）
    int totalNonSupport = 0;
    for (const auto& [pos, count] : distribution) {
        if (pos != Position::SUPPORT) {
            totalNonSupport += count;
        }
    }
    
    // 如果已经有5个非辅助英雄，且新英雄不是辅助，则冲突
    if (totalNonSupport >= 5 && 
        find(hero.positions.begin(), hero.positions.end(), Position::SUPPORT) == hero.positions.end()) {
        return true;
    }
    
    return false;
}

// 获取团队角色分布
unordered_map<Role, int> getTeamRoleDistribution(const vector<int>& team) {
    unordered_map<Role, int> distribution;
    
    // 初始化所有角色计数为0
    for (int i = 0; i < 6; ++i) {
        distribution[static_cast<Role>(i)] = 0;
    }
    
    // 统计每个角色的数量
    for (int id : team) {
        const Hero& hero = heroDB[id];
        for (Role role : hero.roles) {
            distribution[role]++;
        }
    }
    
    return distribution;
}

// 评估阵容平衡性
float evaluateTeamBalance(const vector<int>& team) {
    float balance = 0.0f;
    auto roleDist = getTeamRoleDistribution(team);
    auto posDist = getTeamPositionDistribution(team);
    
    // 1. 角色平衡性评分
    // 坦克和辅助的平衡性
    if (roleDist[Role::TANK] >= 1) balance += 0.2f;
    if (roleDist[Role::SUPPORT] >= 1) balance += 0.2f;
    
    // 输出和控制的平衡性
    if (roleDist[Role::MARKSMAN] >= 1) balance += 0.15f;
    if (roleDist[Role::MAGE] >= 1) balance += 0.15f;
    if (roleDist[Role::ASSASSIN] >= 1) balance += 0.15f;
    if (roleDist[Role::WARRIOR] >= 1) balance += 0.15f;
    
    // 2. 位置平衡性评分
    if (posDist[Position::TOP] >= 1) balance += 0.1f;
    if (posDist[Position::JUNGLE] >= 1) balance += 0.1f;
    if (posDist[Position::APC] >= 1) balance += 0.1f;
    if (posDist[Position::ADC] >= 1) balance += 0.1f;
    if (posDist[Position::SUPPORT] >= 1) balance += 0.1f;
    
    // 3. 角色重复惩罚
    for (const auto& [role, count] : roleDist) {
        if (count > 1) {
            balance -= (count - 1) * 0.1f;
        }
    }
    
    return balance;
}

// 评估阵容强度
float evaluateTeamStrength(const vector<int>& team) {
    float strength = 0.0f;
    
    // 计算团队平均强度
    for (int id : team) {
        strength += heroDB[id].tier;
    }
    strength /= team.size();
    
    // 考虑克制关系
    for (int id : team) {
        const Hero& hero = heroDB[id];
        for (int counter : hero.counters) {
            if (find(team.begin(), team.end(), counter) != team.end()) {
                strength += 0.1f; // 有克制关系加分
            }
        }
    }
    
    return strength;
}

// 评估阵容协同性
float evaluateTeamSynergy(const vector<int>& team) {
    float synergy = 0.0f;
    
    // 计算英雄之间的协同关系
    for (size_t i = 0; i < team.size(); ++i) {
        for (size_t j = i + 1; j < team.size(); ++j) {
            const Hero& hero1 = heroDB[team[i]];
            const Hero& hero2 = heroDB[team[j]];
            
            // 直接协同关系
            if (find(hero1.synergies.begin(), hero1.synergies.end(), team[j]) != hero1.synergies.end()) {
                synergy += 0.3f;
            }
            
            // 角色协同关系
            if ((hero1.roles[0] == Role::TANK && hero2.roles[0] == Role::MARKSMAN) ||
                (hero1.roles[0] == Role::SUPPORT && hero2.roles[0] == Role::MARKSMAN) ||
                (hero1.roles[0] == Role::ASSASSIN && hero2.roles[0] == Role::MAGE)) {
                synergy += 0.2f;
            }
        }
    }
    
    return synergy;
}

// 计算直接克制关系
float calculateDirectCounter(int heroId, int targetId) {
    const Hero& hero = heroDB[heroId];
    const Hero& target = heroDB[targetId];
    
    // 检查直接克制关系
    if (find(hero.counters.begin(), hero.counters.end(), targetId) != hero.counters.end()) {
        return 1.0f; // 直接克制
    }
    
    // 检查被克制关系
    if (find(target.counters.begin(), target.counters.end(), heroId) != target.counters.end()) {
        return -0.8f; // 被克制
    }
    
    return 0.0f;
}

// 计算间接克制关系
float calculateIndirectCounter(int heroId, int targetId) {
    const Hero& hero = heroDB[heroId];
    const Hero& target = heroDB[targetId];
    float counter = 0.0f;
    
    // 检查英雄A克制英雄B的克制者
    for (int counterId : target.counters) {
        if (find(hero.counters.begin(), hero.counters.end(), counterId) != hero.counters.end()) {
            counter += 0.4f; // 间接克制
        }
    }
    
    // 检查英雄A被英雄B的克制者克制
    for (int counterId : hero.counters) {
        if (find(target.counters.begin(), target.counters.end(), counterId) != target.counters.end()) {
            counter -= 0.3f; // 间接被克制
        }
    }
    
    return counter;
}

// 计算角色克制关系
float calculateRoleCounter(int heroId, int targetId) {
    const Hero& hero = heroDB[heroId];
    const Hero& target = heroDB[targetId];
    float counter = 0.0f;
    
    // 角色克制关系
    for (Role heroRole : hero.roles) {
        for (Role targetRole : target.roles) {
            // 刺客克制法师和射手
            if (heroRole == Role::ASSASSIN && 
                (targetRole == Role::MAGE || targetRole == Role::MARKSMAN)) {
                counter += 0.6f;
            }
            // 坦克克制刺客
            if (heroRole == Role::TANK && targetRole == Role::ASSASSIN) {
                counter += 0.5f;
            }
            // 战士克制射手
            if (heroRole == Role::WARRIOR && targetRole == Role::MARKSMAN) {
                counter += 0.4f;
            }
            // 法师克制坦克
            if (heroRole == Role::MAGE && targetRole == Role::TANK) {
                counter += 0.3f;
            }
            // 射手克制战士
            if (heroRole == Role::MARKSMAN && targetRole == Role::WARRIOR) {
                counter += 0.3f;
            }
        }
    }
    
    return counter;
}

// 计算位置克制关系
float calculatePositionCounter(int heroId, int targetId) {
    const Hero& hero = heroDB[heroId];
    const Hero& target = heroDB[targetId];
    float counter = 0.0f;
    
    // 位置克制关系
    for (Position heroPos : hero.positions) {
        for (Position targetPos : target.positions) {
            // 打野克制发育路
            if (heroPos == Position::JUNGLE && targetPos == Position::ADC) {
                counter += 0.4f;
            }
            // 中路克制对抗路
            if (heroPos == Position::APC && targetPos == Position::TOP) {
                counter += 0.3f;
            }
            // 对抗路克制发育路
            if (heroPos == Position::TOP && targetPos == Position::ADC) {
                counter += 0.3f;
            }
            // 辅助克制打野
            if (heroPos == Position::SUPPORT && targetPos == Position::JUNGLE) {
                counter += 0.2f;
            }
        }
    }
    
    return counter;
}

// 计算综合克制关系
float calculateTotalCounter(int heroId, const vector<int>& targets) {
    float totalCounter = 0.0f;
    
    for (int targetId : targets) {
        // 直接克制关系（权重最高）
        totalCounter += calculateDirectCounter(heroId, targetId) * 0.4f;
        
        // 间接克制关系
        totalCounter += calculateIndirectCounter(heroId, targetId) * 0.2f;
        
        // 角色克制关系
        totalCounter += calculateRoleCounter(heroId, targetId) * 0.2f;
        
        // 位置克制关系
        totalCounter += calculatePositionCounter(heroId, targetId) * 0.2f;
    }
    
    return totalCounter;
}

// 计算直接协同关系
float calculateDirectSynergy(int heroId, int allyId) {
    const Hero& hero = heroDB[heroId];
    const Hero& ally = heroDB[allyId];
    
    // 检查直接协同关系
    if (find(hero.synergies.begin(), hero.synergies.end(), allyId) != hero.synergies.end()) {
        return 1.0f; // 直接协同
    }
    
    // 检查被协同关系
    if (find(ally.synergies.begin(), ally.synergies.end(), heroId) != ally.synergies.end()) {
        return 0.8f; // 被协同
    }
    
    return 0.0f;
}

// 计算间接协同关系
float calculateIndirectSynergy(int heroId, int allyId) {
    const Hero& hero = heroDB[heroId];
    const Hero& ally = heroDB[allyId];
    float synergy = 0.0f;
    
    // 检查英雄A协同英雄B的协同者
    for (int synergyId : ally.synergies) {
        if (find(hero.synergies.begin(), hero.synergies.end(), synergyId) != hero.synergies.end()) {
            synergy += 0.4f; // 间接协同
        }
    }
    
    // 检查英雄A被英雄B的协同者协同
    for (int synergyId : hero.synergies) {
        if (find(ally.synergies.begin(), ally.synergies.end(), synergyId) != ally.synergies.end()) {
            synergy += 0.3f; // 间接被协同
        }
    }
    
    return synergy;
}

// 计算角色协同关系
float calculateRoleSynergy(int heroId, int allyId) {
    const Hero& hero = heroDB[heroId];
    const Hero& ally = heroDB[allyId];
    float synergy = 0.0f;
    
    // 角色协同关系
    for (Role heroRole : hero.roles) {
        for (Role allyRole : ally.roles) {
            // 坦克-射手组合
            if ((heroRole == Role::TANK && allyRole == Role::MARKSMAN) ||
                (heroRole == Role::MARKSMAN && allyRole == Role::TANK)) {
                synergy += 0.6f;
            }
            // 辅助-射手组合
            if ((heroRole == Role::SUPPORT && allyRole == Role::MARKSMAN) ||
                (heroRole == Role::MARKSMAN && allyRole == Role::SUPPORT)) {
                synergy += 0.5f;
            }
            // 刺客-法师组合
            if ((heroRole == Role::ASSASSIN && allyRole == Role::MAGE) ||
                (heroRole == Role::MAGE && allyRole == Role::ASSASSIN)) {
                synergy += 0.4f;
            }
            // 战士-法师组合
            if ((heroRole == Role::WARRIOR && allyRole == Role::MAGE) ||
                (heroRole == Role::MAGE && allyRole == Role::WARRIOR)) {
                synergy += 0.3f;
            }
        }
    }
    
    return synergy;
}

// 计算位置协同关系
float calculatePositionSynergy(int heroId, int allyId) {
    const Hero& hero = heroDB[heroId];
    const Hero& ally = heroDB[allyId];
    float synergy = 0.0f;
    
    // 位置协同关系
    for (Position heroPos : hero.positions) {
        for (Position allyPos : ally.positions) {
            // 辅助-射手组合
            if ((heroPos == Position::SUPPORT && allyPos == Position::ADC) ||
                (heroPos == Position::ADC && allyPos == Position::SUPPORT)) {
                synergy += 0.5f;
            }
            // 打野-中路组合
            if ((heroPos == Position::JUNGLE && allyPos == Position::APC) ||
                (heroPos == Position::APC && allyPos == Position::JUNGLE)) {
                synergy += 0.4f;
            }
            // 对抗路-打野组合
            if ((heroPos == Position::TOP && allyPos == Position::JUNGLE) ||
                (heroPos == Position::JUNGLE && allyPos == Position::TOP)) {
                synergy += 0.3f;
            }
        }
    }
    
    return synergy;
}

// 计算技能协同关系
float calculateSkillSynergy(int heroId, int allyId) {
    const Hero& hero = heroDB[heroId];
    const Hero& ally = heroDB[allyId];
    float synergy = 0.0f;
    
    // 控制技能协同
    if ((hero.roles[0] == Role::TANK || hero.roles[0] == Role::SUPPORT) &&
        (ally.roles[0] == Role::MAGE || ally.roles[0] == Role::MARKSMAN)) {
        synergy += 0.4f;
    }
    
    // 输出技能协同
    if ((hero.roles[0] == Role::MAGE || hero.roles[0] == Role::MARKSMAN) &&
        (ally.roles[0] == Role::MAGE || ally.roles[0] == Role::MARKSMAN)) {
        synergy += 0.3f;
    }
    
    // 保护技能协同
    if ((hero.roles[0] == Role::SUPPORT || hero.roles[0] == Role::TANK) &&
        (ally.roles[0] == Role::MARKSMAN || ally.roles[0] == Role::MAGE)) {
        synergy += 0.3f;
    }
    
    return synergy;
}

// 计算综合协同关系
float calculateTotalSynergy(int heroId, const vector<int>& allies) {
    float totalSynergy = 0.0f;
    
    for (int allyId : allies) {
        // 直接协同关系（权重最高）
        totalSynergy += calculateDirectSynergy(heroId, allyId) * 0.3f;
        
        // 间接协同关系
        totalSynergy += calculateIndirectSynergy(heroId, allyId) * 0.2f;
        
        // 角色协同关系
        totalSynergy += calculateRoleSynergy(heroId, allyId) * 0.2f;
        
        // 位置协同关系
        totalSynergy += calculatePositionSynergy(heroId, allyId) * 0.2f;
        
        // 技能协同关系
        totalSynergy += calculateSkillSynergy(heroId, allyId) * 0.1f;
    }
    
    return totalSynergy;
}

// 计算多维度阵容评分
TeamScore calculateTeamScore(const vector<int>& team, const vector<int>& enemies) {
    TeamScore score;
    
    // 计算平衡性评分 (25%)
    score.balance = evaluateTeamBalance(team);
    
    // 计算强度评分 (25%)
    score.strength = evaluateTeamStrength(team);
    
    // 计算协同性评分 (30%)
    score.synergy = evaluateTeamSynergy(team);
    
    // 计算克制评分 (20%)
    float totalCounter = 0.0f;
    for (int heroId : team) {
        totalCounter += calculateTotalCounter(heroId, enemies);
    }
    score.counter = totalCounter / team.size();
    
    // 计算总分（使用调整后的权重）
    score.total = score.balance * 0.25f +    // 平衡性权重从0.2提升到0.25
                 score.strength * 0.25f +    // 强度权重从0.3降低到0.25
                 score.synergy * 0.30f +     // 协同性权重从0.25提升到0.30
                 score.counter * 0.20f;      // 克制权重从0.25降低到0.20
    
    return score;
}

// 修改建议选择英雄函数，使用多维度评分系统
vector<int> suggestPick() {
    priority_queue<pair<float,int>> scores;

    for (auto& [id, hero] : heroDB) {
        if (isAlreadyPickedOrBanned(id)) continue;
        if (hasPositionConflict(id, allyPicks)) continue;

        // 构建潜在团队
        vector<int> potentialTeam = allyPicks;
        potentialTeam.push_back(id);
        
        // 计算多维度评分
        TeamScore teamScore = calculateTeamScore(potentialTeam, enemyPicks);
        
        // 使用总分作为最终评分
        scores.push({teamScore.total, id});
    }
    
    return topN(scores, 5);
}



