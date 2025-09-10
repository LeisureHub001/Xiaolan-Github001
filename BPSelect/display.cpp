#include "display.h"
#include <iomanip>
#include <iostream>
#include <vector>
#include <algorithm>

void displayMenu() {
    cout << "\n==== 英雄选择助手 ====\n"
         << "1. 显示所有英雄\n"
         << "2. 添加禁用英雄\n"
         << "3. 添加我方选择\n"
         << "4. 添加敌方选择\n"
         << "5. 获取禁用建议\n"
         << "6. 获取选择建议\n"
         << "7. 显示当前状态\n"
         << "8. 重置选择\n"
         << "0. 退出\n"
         << "请输入选项: ";
}

void displayHero(const Hero& hero) {
    cout << "ID: " << setw(3) << hero.id << " | "
         << "名称: " << setw(10) << hero.name << " | "
         << "强度: " << fixed << setprecision(1) << hero.tier << endl;
}

void displayHeroList() {
    cout << "\n=== 英雄列表 ===\n";
    cout << left 
         << setw(6) << "ID" << " | "
         << setw(8) << "名称" << " | "
         << setw(18) << "角色" << " | "
         << setw(18) << "位置" << " | "
         << setw(6) << "强度" << endl;
    cout << string(64, '=') << endl;
    
    // 将英雄按ID排序
    vector<pair<int, Hero>> sortedHeroes(heroDB.begin(), heroDB.end());
    sort(sortedHeroes.begin(), sortedHeroes.end(), 
         [](const auto& a, const auto& b) { return a.first < b.first; });
    
    for (const auto& [id, hero] : sortedHeroes) {
        cout << left 
             << setw(6) << id << " | "
             << setw(8) << hero.name << " | "
             << setw(18) << hero.getRoleString() << " | "
             << setw(18) << hero.getPositionString() << " | "
             << setw(6) << fixed << setprecision(1) << hero.tier << endl;
    }
}

void showBanSuggestions() {
    cout << "\n=== 禁用建议 ===\n";
    vector<int> suggestions = suggestBan();
    for (int id : suggestions) {
        heroDB.at(id).display();
    }
}

void showPickSuggestions() {
    cout << "\n=== 选择建议 ===\n";
    vector<int> suggestions = suggestPick();
    
    cout << "评分维度说明：\n"
         << "平衡性(25%) - 阵容角色和位置的平衡程度\n"
         << "强度(25%) - 英雄基础强度和克制关系\n"
         << "协同性(30%) - 英雄之间的配合程度\n"
         << "克制(20%) - 对敌方英雄的克制效果\n"
         << "总分 - 综合评分\n\n";
    
    cout << left
         << setw(8) << "英雄" << " | "
         << setw(10) << "平衡性" << " | "
         << setw(10) << "强度" << " | "
         << setw(10) << "协同性" << " | "
         << setw(10) << "克制" << " | "
         << setw(10) << "总分" << endl;
    cout << string(70, '=') << endl;
    
    for (int id : suggestions) {
        vector<int> potentialTeam = allyPicks;
        potentialTeam.push_back(id);
        TeamScore score = calculateTeamScore(potentialTeam, enemyPicks);
        
        cout << left
             << setw(8) << heroDB[id].name << " | "
             << setw(10) << fixed << setprecision(2) << score.balance << " | "
             << setw(10) << fixed << setprecision(2) << score.strength << " | "
             << setw(10) << fixed << setprecision(2) << score.synergy << " | "
             << setw(10) << fixed << setprecision(2) << score.counter << " | "
             << setw(10) << fixed << setprecision(2) << score.total << endl;
    }
}

void showCurrentStatus() {
    cout << "\n=== 当前状态 ===\n";
    
    cout << "禁用英雄: ";
    if (banned.empty()) {
        cout << "无" << endl;
    } else {
        cout << endl;
        cout << left << setw(6) << "ID" << " | " << setw(8) << "名称" << endl;
        cout << string(16, '=') << endl;
        for (int id : banned) {
            cout << left << setw(6) << id << " | " << setw(8) << heroDB[id].name << endl;
        }
    }
    
    cout << "\n我方选择: ";
    if (allyPicks.empty()) {
        cout << "无" << endl;
    } else {
        cout << endl;
        cout << left << setw(6) << "ID" << " | " << setw(8) << "名称" << endl;
        cout << string(16, '=') << endl;
        for (int id : allyPicks) {
            cout << left << setw(6) << id << " | " << setw(8) << heroDB[id].name << endl;
        }
    }
    
    cout << "\n敌方选择: ";
    if (enemyPicks.empty()) {
        cout << "无" << endl;
    } else {
        cout << endl;
        cout << left << setw(6) << "ID" << " | " << setw(8) << "名称" << endl;
        cout << string(16, '=') << endl;
        for (int id : enemyPicks) {
            cout << left << setw(6) << id << " | " << setw(8) << heroDB[id].name << endl;
        }
    }
}

void displayHeroDetails(int id) {
    const Hero& hero = heroDB.at(id);
    cout << "\n=== 英雄详情 ===\n";
    cout << "ID: " << hero.id << endl;
    cout << "名称: " << hero.name << endl;
    cout << "角色: " << hero.getRoleString() << endl;
    cout << "位置: " << hero.getPositionString() << endl;
    cout << "强度: " << fixed << setprecision(1) << hero.tier << endl;
    
    cout << "\n克制英雄: ";
    if (hero.counters.empty()) {
        cout << "无" << endl;
    } else {
        cout << endl;
        for (int counterId : hero.counters) {
            cout << "- " << heroDB[counterId].name << endl;
        }
    }
    
    cout << "\n协同英雄: ";
    if (hero.synergies.empty()) {
        cout << "无" << endl;
    } else {
        cout << endl;
        for (int synergyId : hero.synergies) {
            cout << "- " << heroDB[synergyId].name << endl;
        }
    }
}

void displayHeroSelection(const string& prompt) {
    cout << "\n" << prompt << " (输入0查看英雄列表，输入-1返回上一级): ";
}

void displayHeroAddResult(const Hero& hero, const string& action) {
    cout << "已" << action << ": " << hero.name << endl;
}

void displayTeamAnalysis() {
    cout << "\n=== 团队分析 ===\n";
    TeamScore score = calculateTeamScore(allyPicks, enemyPicks);
    
    cout << "平衡性: " << fixed << setprecision(2) << score.balance << endl;
    cout << "强度: " << score.strength << endl;
    cout << "协同性: " << score.synergy << endl;
    cout << "克制: " << score.counter << endl;
    cout << "总分: " << score.total << endl;
}

void displaySearchInterface() {
    cout << "\n=== 英雄搜索 ===\n"
         << "1. 按名称搜索\n"
         << "2. 按角色搜索\n"
         << "3. 按位置搜索\n"
         << "0. 返回\n"
         << "请输入选项: ";
}

void displayExportOptions() {
    cout << "\n=== 数据导出 ===\n"
         << "1. 导出当前阵容\n"
         << "2. 导出选择历史\n"
         << "3. 导出英雄数据\n"
         << "0. 返回\n"
         << "请输入选项: ";
} 