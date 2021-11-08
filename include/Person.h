//
// Created by skybcyang on 2021/11/8.
//

#ifndef ELEVATE_GAME_PERSON_H
#define ELEVATE_GAME_PERSON_H

#include <random>
#include "GameInfo.h"

struct Person {
    Person(uint32_t floor_num) : aim_floor(floor_num) {
        static std::uniform_int_distribution<uint32_t> distribution(1, GameInfo::building_floor_num);
        static std::default_random_engine engine((uint32_t)time(nullptr));
        while (aim_floor == floor_num) {
            aim_floor = distribution(engine);
        }
    }
    void Print() const {
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
    uint32_t GetAimFloor() {
        return aim_floor;
    }
private:
    uint32_t wait_round = 0;
    uint32_t aim_floor = 0;
};

#endif //ELEVATE_GAME_PERSON_H
