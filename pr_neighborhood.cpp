//
// acquire the neighbors and return the minimum distance one
//

#include <algorithm>
#include <cstdio>

#include "fjsp.h"
#include "solution.h"
#include "schedule.h"
#include "pathrelinking.h"
#include "pr_neighborhood.h"
#include "pr_distance.h"

struct pr_solution pr_neighborhood_position(struct solution S_i, struct solution S_g, int index){
    schedule Sc_g = solution_decode(S_g);
    struct pr_solution S;
    S.distance_sg = -1;
    int length = S_i.os.size();
    int *job_process = new int[num_jobs];
    for(int i = 0; i < num_jobs; i++)
        job_process[i] = 0;
    int *ms = new int[length];
    int *ws = new int[length];
    for(int i = 0; i < length; i++){
        ms[i] = jobs[S_i.os[i]].operations[job_process[S_i.os[i]]].candidate_machine[S_i.ms[i]].id;
        ws[i] = jobs[S_i.os[i]].operations[job_process[S_i.os[i]]].candidate_worker[S_i.ws[i]].id;
        job_process[S_i.os[i]]++;
    }
    for(int i = 0; i < length; i++){
        if(i == index || ws[i] != ws[index] || ms[i] != ms[index])
            continue;
        struct solution S_n = S_i;
        std::swap(S_n.os[i], S_n.os[index]);
        std::swap(S_n.ms[i], S_n.ms[index]);
        std::swap(S_n.ws[i], S_n.ws[index]);
        schedule Sc_n = solution_decode(S_n);
        if(Sc_n.max_time == -1)
        	continue;
        int distance = pr_distance(Sc_n, Sc_g);
        if(distance < S.distance_sg || S.distance_sg == -1){
            S.max_time = Sc_n.max_time;
            S.distance_sg = distance;
            S.solution = S_n;
        }
        clear_schedule(Sc_n);
    }
    delete[] job_process;
    delete[] ms;
    delete[] ws;
    return S;
}

struct pr_solution pr_neighborhood_machine(struct solution S_i, struct solution S_g, int index){
    schedule Sc_g = solution_decode(S_g);
    struct pr_solution S;
    S.distance_sg = -1;
    int index_order = 0;
    for(int i = 0; i < index; i++)
        if(S_i.os[i] == S_i.os[index])
            index_order++;
    int num_candidate_machine = jobs[S_i.os[index]].operations[index_order].candidate_machine.size();
    int ans = -1;
    for(int i = 0; i < num_candidate_machine; i++)
        if(i != S_i.ms[index]){
            struct solution S_n = S_i;
            S_n.ms[index] = i;
            schedule Sc_n = solution_decode(S_n);
            if(Sc_n.max_time == -1)
        		continue;
            int distance = pr_distance(Sc_n, Sc_g);
            if(distance < S.distance_sg || S.distance_sg == -1){
                S.max_time = Sc_n.max_time;
                S.distance_sg = distance;
                S.solution = S_n;
                ans = pr_distance(Sc_n, Sc_g);
            }
            clear_schedule(Sc_n);
        }
    return S;
}

struct pr_solution pr_neighborhood_worker(struct solution S_i, struct solution S_g, int index){
    schedule Sc_g = solution_decode(S_g);
    struct pr_solution S;
    S.distance_sg = -1;
    int index_order = 0;
    for(int i = 0; i < index; i++)
        if(S_i.os[i] == S_i.os[index])
            index_order++;
    int num_candidate_worker = jobs[S_i.os[index]].operations[index_order].candidate_worker.size();
    int ans = -1;
    for(int i = 0; i < num_candidate_worker; i++){
        if(i != S_i.ws[index]){
            struct solution S_n = S_i;
            S_n.ws[index] = i;
            schedule Sc_n = solution_decode(S_n);
            if(Sc_n.max_time == -1)
        		continue;
            int distance = pr_distance(Sc_n, Sc_g);
            if(distance < S.distance_sg || S.distance_sg == -1){
                S.max_time = Sc_n.max_time;
                S.distance_sg = distance;
                S.solution = S_n;
            }
            clear_schedule(Sc_n);
        }
    }
    return S;
}
