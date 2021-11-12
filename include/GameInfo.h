//
// Created by skybcyang on 2021/11/8.
//

#ifndef ELEVATE_GAME_GAMEINFO_H
#define ELEVATE_GAME_GAMEINFO_H

enum Instruction {
    INVALID,
    UP,
    DONW,
    OPEN,
    CLOSE,
};

struct GameInfo {
    static constexpr uint32_t building_floor_num = 2; // 楼层数
    static constexpr uint32_t building_elevator_num = 2; // 电梯数
    static constexpr uint32_t elevator_load_max_num = 16; // 电梯最大载人数
    static constexpr uint32_t elevator_run_round = 100; // 回合数

    uint32_t round = elevator_run_round;
    std::atomic<Instruction> current_instruction[building_floor_num]{};
    std::atomic_bool game_over = false;
};

#endif //ELEVATE_GAME_GAMEINFO_H
