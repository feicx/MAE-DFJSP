//
// Path Relinking Algorithm
//

#include <algorithm>
#include <vector>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <cstdio>

#include "const.h"
#include "fjsp.h"
#include "solution.h"
#include "schedule.h"
#include "pathrelinking.h"
#include "pr_neighborhood.h"
#include "pr_distance.h"

bool pr_cmp_distance(const struct pr_solution &a, const struct pr_solution &b){
    return a.distance_sg < b.distance_sg;
}

bool pr_cmp_estimate(const struct pr_solution &a, const struct pr_solution &b){
    return a.max_time < b.max_time;
}

bool pr_cmp_sumindex(const struct pr_solution &a, const struct pr_solution &b){
    return a.index_dis + a.index_obj < b.index_dis + b.index_obj;
}

struct solution path_relinking(struct solution S_i, struct solution S_g){
    struct solution S_c = S_i;
    schedule Sc_i, Sc_c, Sc_g;
    std::vector<struct pr_solution> pathSet, N;
    Sc_i = solution_decode(S_i);
    Sc_g = solution_decode(S_g);
    Sc_c = solution_decode(S_c);
    int distance_ig = pr_distance(Sc_i, Sc_g);
    int distance_cg = pr_distance(Sc_c, Sc_g);
    while(distance_cg > alpha * distance_ig){
        int length_sc = S_i.os.size();
        int *count_sc = new int[num_jobs + 5];
        for(int i = 0; i < num_jobs + 5; i++)
            count_sc[i] = 0;
        for(int i = 0; i < length_sc; i++){
            count_sc[S_c.os[i]]++;
            auto ope_i = std::make_pair(S_c.os[i],count_sc[S_c.os[i]]);
            if(Sc_c.schedule[ope_i].machine_id != Sc_g.schedule[ope_i].machine_id){
                struct pr_solution S_min = pr_neighborhood_machine(S_c, S_g, i);
                if(S_min.distance_sg < distance_cg && S_min.distance_sg != -1)
                	if(solution_verify(S_min.solution))
                    	N.push_back(S_min);
            }
            if(Sc_c.schedule[ope_i].worker_id != Sc_g.schedule[ope_i].worker_id){
                struct pr_solution S_min = pr_neighborhood_worker(S_c, S_g, i);
                if(S_min.distance_sg < distance_cg && S_min.distance_sg != -1)
                    if(solution_verify(S_min.solution))
						N.push_back(S_min);
            }
            if(Sc_c.schedule[ope_i].begin_time + Sc_c.schedule[ope_i].end_time !=\
               Sc_g.schedule[ope_i].begin_time + Sc_g.schedule[ope_i].end_time &&\
               Sc_c.schedule[ope_i].machine_id == Sc_g.schedule[ope_i].machine_id &&\
               Sc_c.schedule[ope_i].worker_id == Sc_g.schedule[ope_i].worker_id){
                struct pr_solution S_min = pr_neighborhood_position(S_c, S_g, i);
                if(S_min.distance_sg < distance_cg && S_min.distance_sg != -1)
                	if(solution_verify(S_min.solution))
                    	N.push_back(S_min);
            }
        }
        if(N.size() == 0)
    		return S_c;
        if(N.size() == 1){
            //printf("Can't find!\n");
            return N[0].solution;
        }
        delete[] count_sc;
        std::sort(N.begin(), N.end(), pr_cmp_distance);
        int length_N = N.size();
        for(int i = 0; i < length_N; i++)
            N[i].index_dis = i;
        std::sort(N.begin(), N.end(), pr_cmp_estimate);
        for(int i = 0; i < length_N; i++)
            N[i].index_obj = i;
        std::sort(N.begin(), N.end(), pr_cmp_sumindex);
        srand(time(NULL));
        int k = rand() % std::min(length_N - 1, gamma);
        S_c = N[k].solution;
        clear_schedule(Sc_c);
        Sc_c = solution_decode(S_c);
        distance_cg = pr_distance(Sc_c, Sc_g);
        N.clear();
        if(distance_cg < beta * distance_ig) {
            pr_solution S_c_pr;
            S_c_pr.distance_sg = distance_cg;
            S_c_pr.max_time = Sc_c.max_time;
            S_c_pr.solution = S_c;
            pathSet.push_back(S_c_pr);
        }
    }
	if(pathSet.size() == 0)
		return S_c; 
    std::sort(pathSet.begin(), pathSet.end(), pr_cmp_estimate);
    clear_schedule(Sc_c);
    clear_schedule(Sc_g);
    clear_schedule(Sc_i);
	return pathSet[0].solution;
}