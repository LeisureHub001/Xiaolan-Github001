#include "hero.h"
#include "algorithm.h"
#include "display.h"
#include "input.h"
#include "data/data.h"
#include <iomanip>
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    if (!loadHeroData("data/hero.json")) {
        cerr << "加载英雄数据失败，程序退出\n";
        return 1;
    }
    
    cout << "欢迎使用英雄选择助手！\n";
    
    int choice;
    do {
        displayMenu();
        cin >> choice;
        
        switch(choice) {
            case 1: displayHeroList(); break;
            case 2: addBan(); break;
            case 3: addAllyPick(); break;
            case 4: addEnemyPick(); break;
            case 5: showBanSuggestions(); break;
            case 6: showPickSuggestions(); break;
            case 7: showCurrentStatus(); break;
            case 8: resetSelections(); break;
            case 0: cout << "感谢使用！\n"; break;
            default: cout << "无效选项\n";
        }
    } while (choice != 0);
    
    return 0;
} 