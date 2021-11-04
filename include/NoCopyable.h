//
// Created by skybcyang on 2020/11/29.
//

#ifndef ELEVATOR_NOCOPYABLE_H
#define ELEVATOR_NOCOPYABLE_H

class NoCopyable
{
protected:
    NoCopyable() = default;
    NoCopyable(NoCopyable&&) = default;
    NoCopyable& operator=(NoCopyable&&) = default;
private:
    NoCopyable(const NoCopyable&);
    const NoCopyable& operator=(const NoCopyable&);
};

#endif //ELEVATOR_NOCOPYABLE_H
