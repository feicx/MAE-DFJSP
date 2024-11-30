//
// Problem Definition
//

#ifndef CODE_FJSP_H
#define CODE_FJSP_H

#include<vector>

struct efficient_node{
    int id;
    double factor;
};

struct operation_node{
    int order;
    int time;
    std::vector<struct efficient_node> candidate_machine;
    std::vector<struct efficient_node> candidate_worker;
};

struct job_node{
    int id, num_operations;
    std::vector<struct operation_node> operations;
};

extern int num_jobs, num_machines, num_workers;
extern std::vector<job_node> jobs;

#endif //CODE_FJSP_H
