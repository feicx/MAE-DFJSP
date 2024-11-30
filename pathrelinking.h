//
// Path Relinking Algorithm
//

#ifndef CODE_PATHRELINKING_H
#define CODE_PATHRELINKING_H

#include "fjsp.h"
#include "solution.h"

struct pr_solution{
    struct solution solution;
    int distance_sg;
    int max_time;
    int index_dis;
    int index_obj;
};

struct solution path_relinking(struct solution S_i, struct solution S_g);

#endif //CODE_PATHRELINKING_H
