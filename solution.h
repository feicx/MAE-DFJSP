//
// Solution Definition
//

#ifndef CODE_SOLUTION_H
#define CODE_SOLUTION_H

#include "schedule.h"
#include <vector>

struct solution{
    std::vector<int> os;
    std::vector<int> ms;
    std::vector<int> ws;
    struct solution& operator=(const struct solution &S);
};

struct time_interval{
    int begin_time;
    int end_time;
    bool operator<(const struct time_interval &con) const{
        return this->begin_time < con.begin_time;
    };
};


bool solution_verify(struct solution &S);
schedule solution_decode(struct solution &S, bool out_debug = false);
struct solution solution_init();

extern int decode_max_ans;

#endif //CODE_SOLUTION_H
