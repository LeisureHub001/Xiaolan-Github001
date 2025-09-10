#include "input.h"
#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>

using namespace std;

void addBan() {
    int id;
    displayHeroSelection("请输入要禁用的英雄ID");
    cin >> id;
    
    if (id == -1) return;
    if (id == 0) {
        displayHeroList();
        addBan();
        return;
    }
    
    if (heroDB.find(id) == heroDB.end()) {
        cout << "无效的英雄ID\n";
        addBan();
        return;
    }
    
    if (find(banned.begin(), banned.end(), id) != banned.end()) {
        cout << "该英雄已被禁用\n";
        addBan();
        return;
    }
    
    banned.push_back(id);
    displayHeroAddResult(heroDB[id], "禁用");
}

void addAllyPick() {
    int id;
    displayHeroSelection("请输入我方选择的英雄ID");
    cin >> id;
    
    if (id == -1) return;
    if (id == 0) {
        displayHeroList();
        addAllyPick();
        return;
    }
    
    if (heroDB.find(id) == heroDB.end()) {
        cout << "无效的英雄ID\n";
        addAllyPick();
        return;
    }
    
    if (find(allyPicks.begin(), allyPicks.end(), id) != allyPicks.end()) {
        cout << "该英雄已被选择\n";
        addAllyPick();
        return;
    }
    
    allyPicks.push_back(id);
    displayHeroAddResult(heroDB[id], "选择");
}

void addEnemyPick() {
    int id;
    displayHeroSelection("请输入敌方选择的英雄ID");
    cin >> id;
    
    if (id == -1) return;
    if (id == 0) {
        displayHeroList();
        addEnemyPick();
        return;
    }
    
    if (heroDB.find(id) == heroDB.end()) {
        cout << "无效的英雄ID\n";
        addEnemyPick();
        return;
    }
    
    if (find(enemyPicks.begin(), enemyPicks.end(), id) != enemyPicks.end()) {
        cout << "该英雄已被选择\n";
        addEnemyPick();
        return;
    }
    
    enemyPicks.push_back(id);
    displayHeroAddResult(heroDB[id], "选择");
}

void resetSelections() {
    banned.clear();
    allyPicks.clear();
    enemyPicks.clear();
    cout << "已重置所有选择\n";
}

void handleHeroSelection(int& id, const string& action) {
    displayHeroSelection("请输入要" + action + "的英雄ID");
    cin >> id;
    
    if (id == -1) return;
    if (id == 0) {
        displayHeroList();
        handleHeroSelection(id, action);
        return;
    }
    
    if (heroDB.find(id) == heroDB.end()) {
        cout << "无效的英雄ID\n";
        handleHeroSelection(id, action);
        return;
    }
}

void handleContinueSelection(const string& action) {
    char choice;
    cout << "是否继续" << action << "? (y/n): ";
    cin >> choice;
    
    if (tolower(choice) == 'y') {
        if (action == "禁用") addBan();
        else if (action == "选择") addAllyPick();
        else if (action == "选择敌方") addEnemyPick();
    }
}

void handleHeroSearch() {
    int choice;
    displaySearchInterface();
    cin >> choice;
    
    switch (choice) {
        case 1: {
            string name;
            cout << "请输入英雄名称: ";
            cin >> name;
            
            // 转换为小写以进行不区分大小写的搜索
            transform(name.begin(), name.end(), name.begin(), ::tolower);
            
            bool found = false;
            for (const auto& [id, hero] : heroDB) {
                string heroName = hero.name;
                transform(heroName.begin(), heroName.end(), heroName.begin(), ::tolower);
                
                if (heroName.find(name) != string::npos) {
                    displayHero(hero);
                    found = true;
                }
            }
            
            if (!found) {
                cout << "未找到匹配的英雄\n";
            }
            break;
        }
        case 2: {
            cout << "请选择角色:\n"
                 << "1. 战士\n"
                 << "2. 法师\n"
                 << "3. 刺客\n"
                 << "4. 坦克\n"
                 << "5. 射手\n"
                 << "6. 辅助\n"
                 << "请输入选项: ";
            int roleChoice;
            cin >> roleChoice;
            
            Role role;
            switch (roleChoice) {
                case 1: role = Role::WARRIOR; break;
                case 2: role = Role::MAGE; break;
                case 3: role = Role::ASSASSIN; break;
                case 4: role = Role::TANK; break;
                case 5: role = Role::MARKSMAN; break;
                case 6: role = Role::SUPPORT; break;
                default:
                    cout << "无效的选项\n";
                    return;
            }
            
            bool found = false;
            for (const auto& [id, hero] : heroDB) {
                if (find(hero.roles.begin(), hero.roles.end(), role) != hero.roles.end()) {
                    displayHero(hero);
                    found = true;
                }
            }
            
            if (!found) {
                cout << "未找到匹配的英雄\n";
            }
            break;
        }
        case 3: {
            cout << "请选择位置:\n"
                 << "1. 对抗路\n"
                 << "2. 打野\n"
                 << "3. 中路\n"
                 << "4. 发育路\n"
                 << "5. 游走\n"
                 << "请输入选项: ";
            int posChoice;
            cin >> posChoice;
            
            Position pos;
            switch (posChoice) {
                case 1: pos = Position::TOP; break;
                case 2: pos = Position::JUNGLE; break;
                case 3: pos = Position::APC; break;
                case 4: pos = Position::ADC; break;
                case 5: pos = Position::SUPPORT; break;
                default:
                    cout << "无效的选项\n";
                    return;
            }
            
            bool found = false;
            for (const auto& [id, hero] : heroDB) {
                if (find(hero.positions.begin(), hero.positions.end(), pos) != hero.positions.end()) {
                    displayHero(hero);
                    found = true;
                }
            }
            
            if (!found) {
                cout << "未找到匹配的英雄\n";
            }
            break;
        }
        case 0:
            return;
        default:
            cout << "无效的选项\n";
            break;
    }
}

void handleHeroDetails() {
    int id;
    displayHeroSelection("请输入要查看的英雄ID");
    cin >> id;
    
    if (id == -1) return;
    if (id == 0) {
        displayHeroList();
        handleHeroDetails();
        return;
    }
    
    if (heroDB.find(id) == heroDB.end()) {
        cout << "无效的英雄ID\n";
        handleHeroDetails();
        return;
    }
    
    displayHeroDetails(id);
}

void handleTeamAnalysis() {
    if (allyPicks.empty()) {
        cout << "请先选择我方英雄\n";
        return;
    }
    
    displayTeamAnalysis();
}

void handleSelectionHistory() {
    cout << "\n=== 选择历史 ===\n";
    // TODO: 实现选择历史功能
}

void handleDataExport() {
    int choice;
    displayExportOptions();
    cin >> choice;
    
    switch (choice) {
        case 1:
            // TODO: 导出当前阵容
            break;
        case 2:
            // TODO: 导出选择历史
            break;
        case 3:
            // TODO: 导出英雄数据
            break;
        case 0:
            return;
        default:
            cout << "无效的选项\n";
            break;
    }
}

void saveSelectionHistory() {
    // TODO: 实现保存选择历史功能
}

void loadSelectionHistory() {
    // TODO: 实现加载选择历史功能
} 