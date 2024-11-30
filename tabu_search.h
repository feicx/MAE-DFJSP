//
// tabu search algorithm
//

#ifndef CODE_TABU_SEARCH_H
#define CODE_TABU_SEARCH_H

#include <vector>

#include "ts_tabulist.h"
#include "solution.h"

struct ts_solution{
    int max_time, kind;
    std::vector<ts_tabu_resource_node> tabu_arrange;
    struct solution solution;
    bool operator<(const ts_solution &S) const;
};

struct solution tabu_search(struct solution S_i);

#endif //CODE_TABU_SEARCH_H
