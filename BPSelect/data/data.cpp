#include "data.h"
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

Role stringToRole(const string& roleStr) {
    if (roleStr == "WARRIOR") return Role::WARRIOR;
    if (roleStr == "MAGE") return Role::MAGE;
    if (roleStr == "ASSASSIN") return Role::ASSASSIN;
    if (roleStr == "TANK") return Role::TANK;
    if (roleStr == "MARKSMAN") return Role::MARKSMAN;
    if (roleStr == "SUPPORT") return Role::SUPPORT;
    throw runtime_error("Invalid role: " + roleStr);
}

Position stringToPosition(const string& posStr) {
    if (posStr == "TOP") return Position::TOP;
    if (posStr == "JUNGLE") return Position::JUNGLE;
    if (posStr == "APC") return Position::APC;
    if (posStr == "ADC") return Position::ADC;
    if (posStr == "SUPPORT") return Position::SUPPORT;
    throw runtime_error("Invalid position: " + posStr);
}

bool loadHeroData(const string& filename) {
    try {
        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "无法打开文件: " << filename << endl;
            return false;
        }

        json j;
        file >> j;

        for (const auto& heroJson : j) {
            Hero hero;
            hero.id = heroJson["id"];
            hero.name = heroJson["name"];
            
            // 加载角色类型
            for (const auto& roleStr : heroJson["roles"]) {
                hero.roles.push_back(stringToRole(roleStr));
            }
            
            // 加载位置信息
            for (const auto& posStr : heroJson["positions"]) {
                hero.positions.push_back(stringToPosition(posStr));
            }
            
            // 加载克制和协同英雄
            hero.counters = heroJson["counters"].get<vector<int>>();
            hero.synergies = heroJson["synergies"].get<vector<int>>();
            hero.tier = heroJson["tier"];
            
            heroDB[hero.id] = hero;
        }
        return true;
    } catch (const exception& e) {
        cerr << "加载英雄数据失败: " << e.what() << endl;
        return false;
    }
} 
