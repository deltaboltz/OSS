//
// Created by Connor on 10/20/2020.
//

#ifndef PROJECT3_CLOCK_WORK_H
#define PROJECT3_CLOCK_WORK_H

struct clock
{
    int clockSec;
    int clockNano;

    clock()
    {
        clockSec = 0;
        clockNano = 0;
    }

    void increment(int nano);
    float nextrand(int maxNano);
};

void clock::increment(int nano)
{
        this->clockNano += nano;
        while(this->clockNano > 1e9)
        {
            this->clockNano -= 1e9;
            this->clockSec += 1;
        }
}

float clock::nextrand(int maxNano)
{
    clock copy;
    copy.clockSec = this->clockSec;
    copy.clockNano = this->clockNano;
    copy.increment(rand() % maxNano);
    return (float)copy;
}


#endif //PROJECT3_CLOCK_WORK_H
