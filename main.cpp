//
// Created by skybcyang on 2021/11/1.
//

#include <iostream>
#include <vector>
#include <queue>
#include <cstdlib>
#include <thread>

constexpr uint elevator_run_second_per_layer = 3; // 电梯运行一层所需时间
constexpr uint elevator_open_or_close_door_time = 1; // 电梯门打开或者关闭时间
constexpr uint load_person_num_per_second = 2; // 电梯打开时 每秒进入电梯的人数
constexpr uint elevator_load_person_num = 16; // 电梯最大载人数

struct Person {
    int id = 0;
};

struct Floor {
    void Print() {
        std::cout << "[";
        for (auto& person : persons) {
            std::cout << "P ";
        }
        std::cout << "]" << std::endl;
    }
private:
    std::vector<Person> persons;
};

struct Building {
    explicit Building(size_t floorNum) {
        floors = std::move(std::vector<Floor>(floorNum, Floor{}));
    }
    void Print() {
        std::cout << "building has " << floors.size() << "floors" << std::endl;
        for (auto& floor : floors) {
            floor.Print();
        }
    }
private:
    std::vector<Floor> floors;
};

void Print(Building& building, uint round) {
    system("clear");
    std::cout << "Round " << round << std::endl;
    building.Print();
}


int main() {
    Building building(5);
    int round = 100;
    while(round-- > 0) {
        Print(building, round);
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    return 0;
}