//
// neighborhood retention mechanism
//

#ifndef CODE_TS_RETAIN_H
#define CODE_TS_RETAIN_H

#include <set>

#include "tabu_search.h"
#include "ts_retain.h"
#include "solution.h"

struct ts_retain{
    std::set<struct ts_solution> retain_set;
    void add_to_retain(const struct ts_solution &S);
    struct ts_solution pop_random_sol();
};

#endif //CODE_TS_RETAIN_H
