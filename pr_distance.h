//
// Calculate the distance in PR
//

#ifndef CODE_PR_DISTANCE_H
#define CODE_PR_DISTANCE_H

#include "schedule.h"

int pr_distance(const schedule &S1, const schedule &S2);
int pr_distance(const schedule &S1, const schedule &S2, std::pair<int, int> operation);

#endif //CODE_PR_DISTANCE_H
