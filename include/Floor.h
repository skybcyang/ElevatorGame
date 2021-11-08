//
// Created by skybcyang on 2021/11/8.
//

#ifndef ELEVATE_GAME_FLOOR_H
#define ELEVATE_GAME_FLOOR_H

#include <random>
#include <memory>
#include "Person.h"

struct Floor {
    void Print() {
        std::cout << "[";
        for (auto& person : persons) {
            person->Print();
        }
        std::cout << "]\n";
    }

    void Update(uint32_t current_floor) {
        for (auto& person: persons) {
            person->Update();
        }
        static std::uniform_int_distribution<uint32_t> distribution(0, 4);
        static std::default_random_engine engine((uint32_t)time(nullptr));
        if (distribution(engine) == 1) { // 50%
            persons.push_back(std::move(std::make_shared<Person>(current_floor)));
        }
    }

    std::vector<std::shared_ptr<Person>> GetAvaliablePerson(uint32_t person_num) {
        std::vector<std::shared_ptr<Person>> avaliable_person;
        for (auto iter = persons.begin(); iter != persons.end() && person_num > 0;) {
            avaliable_person.push_back(std::move(*iter));
            iter = persons.erase(iter);
            person_num--;
        }
        return std::move(avaliable_person);
    }
private:
    std::vector<std::shared_ptr<Person>> persons;
};

#endif //ELEVATE_GAME_FLOOR_H
