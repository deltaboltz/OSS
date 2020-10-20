//
// Created by Connor on 10/20/2020.
//

#ifndef PROJECT3_CLOCK_WORK_H
#define PROJECT3_CLOCK_WORK_H

#include <stdlib.h>             //srand(), rand()
#include <string.h>               //std::string, to_string()

struct clk{
    int clk_s;
    int clk_n;

    clk::clk() {
        clk_s = 0;
        clk_n = 0;
    }

    float tofloat();
    void inc(int ns);
    void dec(int ns);
    float nextrand(int maxns);
};

float clk::tofloat() {
    return (float)this->clk_s + (float)this->clk_n/(float)1e9;
}



void clk::inc(int ns) {
    this->clk_n += ns;
    while (this->clk_n > 1e9) {
        this->clk_n -= 1e9;
        this->clk_s += 1;
    }
}

void clk::dec(int ns) {
    this->clk_n -= ns;
    while (this->clk_n < 0) {
        this->clk_n += 1e9;
        this->clk_s -= 1;
    }
    if (this->clk_s < 0) {
        this->clk_n = 0;
        this->clk_s = 0;
    }
}

float clk::nextrand(int maxns) {
    clk copy;
    copy.clk_s = this->clk_s;
    copy.clk_n = this->clk_n;
    copy.inc(rand() % maxns);
    return copy.tofloat();
}

#endif //PROJECT3_CLOCK_WORK_H
