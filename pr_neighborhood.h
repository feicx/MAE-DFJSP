//
// acquire the neighbors and return the minimum distance one
//

#ifndef CODE_PR_NEIGHBORHOOD_H
#define CODE_PR_NEIGHBORHOOD_H

struct pr_solution pr_neighborhood_position(struct solution S_i, struct solution S_g, int index);
struct pr_solution pr_neighborhood_machine(struct solution S_i, struct solution S_g, int index);
struct pr_solution pr_neighborhood_worker(struct solution S_i, struct solution S_g, int index);

#endif //CODE_PR_NEIGHBORHOOD_H
