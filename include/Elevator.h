//
// Created by skybcyang on 2021/11/8.
//

#ifndef ELEVATE_GAME_ELEVATOR_H
#define ELEVATE_GAME_ELEVATOR_H

#include "GameInfo.h"
#include "Person.h"

struct Elevator {
    Elevator() {
        persons.reserve(GameInfo::elevator_load_max_num);
    }

    void Update(GameInfo& game_info) {
        switch(game_info.current_instruction) {
            case(UP) :
                if (current_floor < GameInfo::building_floor_num && !door_status) {
                    current_floor++;
                }
                break;
            case(DONW):
                if (current_floor > 1 && !door_status) {
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
        for (auto iter = persons.begin(); iter != persons.end();) {
            if ((*iter)->GetAimFloor() == current_floor && door_status) {
                iter = persons.erase(iter);
            }
            else {
                iter++;
            }
        }
    }

    void LoadPerson(std::vector<std::shared_ptr<Person>> persons_to_load) {
        persons.insert(persons.end(), persons_to_load.begin(), persons_to_load.end());
    }

    uint32_t GetCapability() {
        return GameInfo::elevator_load_max_num - persons.size();
    }

    uint32_t GetCurrentFloor() {
        return current_floor;
    }

    bool IsDoorOpen() {
        return door_status;
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

#endif //ELEVATE_GAME_ELEVATOR_H
