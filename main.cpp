//
// Created by skybcyang on 2021/11/1.
//

#include <iostream>
#include <thread>
#include "Building.h"

void Print(Building& building, GameInfo& game_info) {
    system("clear");
    std::cout << "Round " << game_info.round << "\n";
    building.Print();
    std::cout << "Current Instruction =";
    for (auto& instruction: game_info.current_instruction) {
        std::cout << " " << instruction;
    }
    std::cout << std::endl;
}

struct System {
    void Run(Building& building, GameInfo& game_info) {
        while(game_info.round-- > 0) {
            Print(building, game_info);
            building.Update(game_info);
            std::this_thread::sleep_for(std::chrono::seconds(2));
        }
        game_info.game_over = true;
    }
};

// TODO: 换成键盘监听
struct KeyBoard {
    void Listen(Building& building, GameInfo& game_info) {
        while(!game_info.game_over) {
            char ch;
            std::cin >> ch;
            {
                if (ch == 'w') {
                    game_info.current_instruction[0] = UP;
                }
                else if (ch == 's') {
                    game_info.current_instruction[0] = DONW;
                }
                else if (ch == 'a') {
                    game_info.current_instruction[0] = CLOSE;
                }
                else if (ch == 'd') {
                    game_info.current_instruction[0] = OPEN;
                }
            }
            Print(building, game_info);
        }
    }
};

int main() {
    GameInfo game_info;
    Building building(GameInfo::building_floor_num, GameInfo::building_elevator_num);
    System system;
    KeyBoard key_board;

    std::thread key_board_thread(&KeyBoard::Listen, &key_board, std::ref(building), std::ref(game_info));
    std::thread system_thread(&System::Run, &system, std::ref(building), std::ref(game_info));
    system_thread.join();
    key_board_thread.join();
    return 0;
}