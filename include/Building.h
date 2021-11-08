//
// Created by skybcyang on 2021/11/8.
//

#ifndef ELEVATE_GAME_BUILDING_H
#define ELEVATE_GAME_BUILDING_H

#include <vector>
#include <map>
#include "Elevator.h"
#include "Floor.h"

struct Building {
    explicit Building(uint32_t floorNum, uint32_t elevator_num) {
        floors = std::move(std::vector<Floor>(floorNum, Floor{}));
        elevators = std::move(std::vector<Elevator>(elevator_num, Elevator{}));
    }
    void Update(GameInfo& game_info) {
        // floor, elevator_id, person_num
        std::map<uint32_t, std::map<uint32_t, uint32_t>> capabilitys;
        for(uint32_t elevator_id = 0; elevator_id < GameInfo::building_elevator_num; elevator_id++) {
            elevators[elevator_id].Update(game_info);
            if (!elevators[elevator_id].IsDoorOpen()) {
                continue;
            }
            if (capabilitys.find(elevators[elevator_id].GetCurrentFloor()) != capabilitys.end()) {
                capabilitys.emplace(elevators[elevator_id].GetCurrentFloor(),
                                    std::map<uint32_t, uint32_t>{{elevator_id, elevators[elevator_id].GetCapability()}});
            }
            else {
                capabilitys[elevators[elevator_id].GetCurrentFloor()].insert(
                        {elevator_id, elevators[elevator_id].GetCapability()});
            }
        }

        std::map<uint32_t, std::vector<std::shared_ptr<Person>>> avaliable_person;
        for(uint32_t floor_num = 1; floor_num <= GameInfo::building_floor_num; floor_num++) {
            floors[floor_num - 1].Update(floor_num);
            if (capabilitys.find(floor_num) == capabilitys.end()) {
                continue;
            }

            for (auto& elevator_capability : capabilitys[floor_num]) {
                avaliable_person.emplace(elevator_capability.first, floors[floor_num - 1].GetAvaliablePerson(elevator_capability.second));
            }
        }

        for(uint32_t elevator_id = 0; elevator_id < GameInfo::building_elevator_num; elevator_id++) {
            if (avaliable_person.find(elevator_id) == avaliable_person.end()) {
                continue;
            }
            elevators[elevator_id].LoadPerson(avaliable_person[elevator_id]);
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

#endif //ELEVATE_GAME_BUILDING_H
