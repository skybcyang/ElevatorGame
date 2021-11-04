//
// Created by skybcyang on 2021/11/1.
//

#include <iostream>
#include <vector>
#include <cstdlib>
#include <thread>
#include <random>
#include <NoCopyable.h>

constexpr uint32_t building_floor_num = 10; // 楼层数
constexpr uint32_t building_elevator_num = 2; // 电梯数
constexpr uint32_t elevator_load_max_num = 16; // 电梯最大载人数
constexpr uint32_t elevator_run_round = 100; // 回合数

struct Person : private NoCopyable {
    Person() : NoCopyable() {
        static std::uniform_int_distribution<uint32_t> distribution(1, building_floor_num);
        static std::default_random_engine engine((uint32_t)time(nullptr));
        aim_floor = distribution(engine);
    }
    void Print() {
        if (wait_round < 2 * building_floor_num) {
            std::cout << "\033[32mP\033[0m";
        }
        else if (wait_round < 4 * building_floor_num) {
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
        std::cout << "]" << std::endl;
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
        persons.reserve(elevator_load_max_num);
    }
    void Update() {
        door_status = !door_status;
        if (current_floor > 1) {
            current_floor--;
        }
        if (current_floor < building_floor_num) {
            current_floor++;
        }
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
    void Update() {
        for(auto& floor: floors) {
            floor.Update();
        }
        for(auto& elevator: elevators) {
            elevator.Update();
        }
    }
    void Print() {
        std::cout << "building has " << floors.size() << "floors" << std::endl;
        for (uint32_t floor_num = building_floor_num; floor_num > 0; floor_num--) {
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

void Print(Building& building, uint round) {
    system("clear");
    std::cout << "Round " << round << std::endl;
    building.Print();
}

struct System {
    void Run() {
        Building building(building_floor_num, building_elevator_num);
        uint32_t round = elevator_run_round;
        while(round-- > 0) {
            building.Update();
            Print(building, round);
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
};

struct KeyBoard {
    void Listen() {
    }
};

int main() {
    System system;
    KeyBoard key_board;
    std::thread key_board_thread(&KeyBoard::Listen, &key_board);
    std::thread system_thread(&System::Run, &system);
    system_thread.join();
    key_board_thread.join();
    return 0;
}