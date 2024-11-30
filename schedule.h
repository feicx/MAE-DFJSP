//
// Schedule Definition
//

#ifndef CODE_SCHEDULE_H
#define CODE_SCHEDULE_H

#include<utility>
#include<vector>
#include<map>

struct schedule_node{
    int job_id, jon_order;
    int begin_time, end_time;
    int machine_id;
    int worker_id;
};

struct schedule{
        int max_time;
        int *machine_L = NULL, *worker_L = NULL;
        std::map<std::pair<int, int>, struct schedule_node> schedule;
};

void clear_schedule(struct schedule Sc);

#endif //CODE_SCHEDULE_H
