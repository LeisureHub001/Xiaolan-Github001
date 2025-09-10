#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <iostream>
#include <iomanip>
using namespace std;

// 英雄定位枚举
enum class Role {
    WARRIOR,    // 战士
    MAGE,       // 法师
    ASSASSIN,   // 刺客
    TANK,       // 坦克
    MARKSMAN,   // 射手
    SUPPORT     // 辅助
};

// 分路位置枚举
enum class Position {
    TOP,        // 对抗路
    JUNGLE,     // 打野
    APC,        // 中路
    ADC,        // 发育路
    SUPPORT     // 游走路
};

// 英雄属性结构体
struct Hero {
    int id;
    string name;
    vector<Role> roles;
    vector<Position> positions;
    vector<int> counters;
    vector<int> synergies;
    float tier;

    Hero() = default;
    
    // 显示英雄基本信息
    void display() const {
        cout << "ID: " << setw(3) << id << " | "
             << "名称: " << setw(10) << name << " | "
             << "强度: " << fixed << setprecision(1) << tier << endl;
    }

    // 获取角色字符串
    string getRoleString() const {
        string result;
        for (const auto& role : roles) {
            switch (role) {
                case Role::WARRIOR: result += "战士 "; break;
                case Role::MAGE: result += "法师 "; break;
                case Role::ASSASSIN: result += "刺客 "; break;
                case Role::TANK: result += "坦克 "; break;
                case Role::MARKSMAN: result += "射手 "; break;
                case Role::SUPPORT: result += "辅助 "; break;
            }
        }
        return result;
    }

    // 获取位置字符串
    string getPositionString() const {
        string result;
        for (const auto& pos : positions) {
            switch (pos) {
                case Position::TOP: result += "对抗路 "; break;
                case Position::JUNGLE: result += "打野 "; break;
                case Position::APC: result += "中路 "; break;
                case Position::ADC: result += "发育路 "; break;
                case Position::SUPPORT: result += "游走 "; break;
            }
        }
        return result;
    }
};

// 全局容器声明
extern unordered_map<int, Hero> heroDB;
extern vector<int> banned;
extern vector<int> allyPicks;
extern vector<int> enemyPicks;
