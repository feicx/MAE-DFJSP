//
// tabu search neighbourhood struct
//

#ifndef CODE_TS_NEIGHBORHOOD_H
#define CODE_TS_NEIGHBORHOOD_H


#include "solution.h"
#include "tabu_search.h"
#include "ts_tabulist.h"

struct ts_solution ts_neighborhood_swap(struct solution &S);
struct ts_solution ts_neighborhood_insert(struct solution &S);
struct ts_solution ts_neighborhood_machine(struct solution &S);
struct ts_solution ts_neighborhood_worker(struct solution &S);

#endif //CODE_TS_NEIGHBORHOOD_H
