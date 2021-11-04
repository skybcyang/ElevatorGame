//
// Created by skybcyang on 2021/11/1.
//

#include <iostream>
#include <vector>
#include <cstdlib>
#include <thread>
#include <future>
#include <random>
#include <atomic>
#include <NoCopyable.h>

enum Instruction {
    INVALID,
    UP,
    DONW,
    OPEN,
    CLOSE,
};

struct GameInfo {
    static constexpr uint32_t building_floor_num = 10; // 楼层数
    static constexpr uint32_t building_elevator_num = 1; // 电梯数
    static constexpr uint32_t elevator_load_max_num = 16; // 电梯最大载人数
    static constexpr uint32_t elevator_run_round = 100; // 回合数

    std::atomic_uint32_t round = elevator_run_round;
    std::atomic<Instruction> current_instruction = INVALID;
    std::atomic_bool game_over = false;
};

struct Person : private NoCopyable {
    Person() : NoCopyable() {
        static std::uniform_int_distribution<uint32_t> distribution(1, GameInfo::building_floor_num);
        static std::default_random_engine engine((uint32_t)time(nullptr));
        aim_floor = distribution(engine);
    }
    void Print() {
        if (wait_round < 2 * GameInfo::building_floor_num) {
            std::cout << "\033[32mP\033[0m";
        }
        else if (wait_round < 4 * GameInfo::building_floor_num) {
            std::cout << "\033[33mP\033[0m";
        }
        else {
            std::cout << "\033[31mP\033[0m";
        }
    }
    void Update() {
        wait_round++;
    }
private:
    uint32_t wait_round = 0;
    uint32_t aim_floor = 0;
};

struct Floor {
    void Print() {
        std::cout << "[";
        for (auto& person : persons) {
            person->Print();
        }
        std::cout << "]\n";
    }
    void Update() {
        for (auto& person: persons) {
            person->Update();
        }
        static std::uniform_int_distribution<uint32_t> distribution(0, 1);
        static std::default_random_engine engine((uint32_t)time(nullptr));
        if (distribution(engine) == 1) { // 50%
            persons.push_back(std::move(std::make_shared<Person>()));
        }
    }
private:
    std::vector<std::shared_ptr<Person>> persons;
};

struct Elevator {
    Elevator() {
        persons.reserve(GameInfo::elevator_load_max_num);
    }
    void Update(GameInfo& game_info) {
        switch(game_info.current_instruction) {
            case(UP) :
                if (current_floor < GameInfo::building_floor_num) {
                    current_floor++;
                }
                break;
            case(DONW):
                if (current_floor > 1) {
                    current_floor--;
                }
                break;
            case(OPEN):
                door_status = true;
                break;
            case(CLOSE):
                door_status = false;
                break;
            default:
                break;
        }
        game_info.current_instruction = INVALID;
    }
    uint32_t GetPersonNum() {
        return persons.size();
    }
    uint32_t GetCurrentFloor() {
        return current_floor;
    }
    void Print() {
        if (door_status) {
            std::cout << "\033[32m[\033[0m" << persons.size() << "\033[32m]\033[0m";
        }
        else {
            std::cout << "[" << persons.size() << "]";
        }
    }
private:
    std::vector<std::shared_ptr<Person>> persons;
    bool door_status = false;
    uint32_t current_floor = 1; // 楼层从1开始
};

struct Building {
    explicit Building(uint32_t floorNum, uint32_t elevator_num) {
        floors = std::move(std::vector<Floor>(floorNum, Floor{}));
        elevators = std::move(std::vector<Elevator>(elevator_num, Elevator{}));
    }
    void Update(GameInfo& game_info) {
        for(auto& floor: floors) {
            floor.Update();
        }
        for(auto& elevator: elevators) {
            elevator.Update(game_info);
        }
    }
    void Print() {
        std::cout << "building has " << floors.size() << "floors" << std::endl;
        for (uint32_t floor_num = GameInfo::building_floor_num; floor_num > 0; floor_num--) {
            for (auto& elevator: elevators) {
                if (elevator.GetCurrentFloor() == floor_num) {
                    elevator.Print();
                }
                std::cout << "\t";
            }
            floors[floor_num - 1].Print();
        }
    }
private:
    std::vector<Floor> floors;
    std::vector<Elevator> elevators;
};

void Print(Building& building, GameInfo& game_info) {
    system("clear");
    std::cout << "Round " << game_info.round << "\n";
    building.Print();
    std::cout << "Current Instruction = " << game_info.current_instruction << std::endl;
}

struct System {
    void Run(Building& building, GameInfo& game_info) {
        while(game_info.round-- > 0) {
            Print(building, game_info);
            building.Update(game_info);
            std::this_thread::sleep_for(std::chrono::seconds(1));
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
                    game_info.current_instruction = UP;
                }
                else if (ch == 's') {
                    game_info.current_instruction = DONW;
                }
                else if (ch == 'a') {
                    game_info.current_instruction = CLOSE;
                }
                else if (ch == 'd') {
                    game_info.current_instruction = OPEN;
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