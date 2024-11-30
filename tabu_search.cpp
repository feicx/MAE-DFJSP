//
// tabu search algorithm
//

#include <vector>
#include <cstring>
#include <cstdio>

#include "const.h"
#include "schedule.h"
#include "solution.h"
#include "tabu_search.h"
#include "ts_retain.h"
#include "ts_neighborhood.h"
#include "ts_tabulist.h"

bool ts_solution::operator<(const ts_solution &S) const{
    return this->max_time < S.max_time;
}

struct solution tabu_search(struct solution S_i){
    int gen = 0;
    ts_tabulist_order tb_order;
    ts_tabulist_resource tb_machine, tb_worker;
    ts_retain retain_set;
    schedule Sc_i = solution_decode(S_i);
    do{
        // Neighbourhood Generate
        std::vector<ts_solution> neighbors;
        int T = t_neighbor_num / 4;
        //printf("N - 1\n");
        while(T--){
            ts_solution S_n = ts_neighborhood_insert(S_i);
            S_n.kind = 1;
            //printf("NS1: %d\n", S_n.max_time);
            if(S_n.max_time != -1)
                neighbors.push_back(S_n);
        }
        //printf("N - 2\n");
        T = t_neighbor_num / 4;
        while(T--){
        	//printf("N - 2 - %d\n", T);
            ts_solution S_n = ts_neighborhood_machine(S_i);
            S_n.kind = 1;
            //printf("NS2: %d\n", S_n.max_time);
            if(S_n.max_time != -1)
                neighbors.push_back(S_n);
        }
        //printf("N - 3\n");
        T = t_neighbor_num / 4;
        while(T--){
            ts_solution S_n = ts_neighborhood_swap(S_i);
            S_n.kind = 2;
            //printf("NS3: %d\n", S_n.max_time);
            if(S_n.max_time != -1)
                neighbors.push_back(S_n);
        }
        T = t_neighbor_num / 4;
        //printf("N - 4\n");
        while(T--){
        	//printf("N - 4 - %d\n", T);
            ts_solution S_n = ts_neighborhood_worker(S_i);
            S_n.kind = 3;
            //printf("NS4: %d\n", S_n.max_time);
            if(S_n.max_time != -1)
                neighbors.push_back(S_n);
        }
        //printf("A: %d\n", neighbors.size());
        //printf("N - 5\n");
        // Tabu Judgment & Select the Best Solution
        int neighbors_num = neighbors.size();
        bool *in_tabu_list = new bool[neighbors_num];
        for(int i = 0; i < neighbors_num; i++)
            in_tabu_list[i] = 0;
        int best_sol_index = -1;
        for(int i = 0; i < neighbors_num; i++){
            if(neighbors[i].max_time < Sc_i.max_time)
                if(best_sol_index == -1 || neighbors[i].max_time < neighbors[best_sol_index].max_time)
                    best_sol_index = i;
            if(neighbors[i].kind == 1){
                //printf("1 - 1\n");
                if(tb_order.in_tabulist(neighbors[i].solution.os)){
                    in_tabu_list[i] = true;
                    continue;
                }
                //printf("1 - 2\n");
            }
            if(neighbors[i].kind == 2){
                //printf("2 - 1\n");
                int num_tabu_ope = neighbors[i].tabu_arrange.size();
                for(int j = 0; j <= num_tabu_ope; j++)
                    if(tb_machine.in_tabulist(neighbors[i].tabu_arrange[j])){
                        in_tabu_list[i] = true;
                        break;
                    }
                //printf("2 - 2\n");
                if(in_tabu_list[i] == true)
                    continue;
            }
            if(neighbors[i].kind == 3){
                //printf("3 - 1\n");
                int num_tabu_ope = neighbors[i].tabu_arrange.size();
                for(int j = 0; j <= num_tabu_ope; j++)
                    if(tb_worker.in_tabulist(neighbors[i].tabu_arrange[j])){
                        in_tabu_list[i] = true;
                        break;
                    }
                if(in_tabu_list[i] == true)
                    continue;
                //printf("3 - 2\n");
            }
            if(best_sol_index == -1 || neighbors[i].max_time < neighbors[best_sol_index].max_time)
                best_sol_index = i;
        }
        delete[] in_tabu_list;
        //printf("Good\n");
        // Update the Retain Set and Tabu List
        if(best_sol_index == -1){
            struct ts_solution S_tc = retain_set.pop_random_sol();
            if(S_tc.kind == 1)
                tb_order.add_tabu_os(S_tc.solution.os);
            if(S_tc.kind == 2)
                for(auto it = S_tc.tabu_arrange.begin(); it != S_tc.tabu_arrange.end(); it++)
                    tb_machine.add_tabu_os(*it);
            if(S_tc.kind == 3)
                for(auto it = S_tc.tabu_arrange.begin(); it != S_tc.tabu_arrange.end(); it++)
                    tb_worker.add_tabu_os(*it);
            if(S_tc.max_time != -1)
            	S_i = S_tc.solution;
            else if(neighbors.size() != 0) 
            	S_i = neighbors[0].solution;
        }else{
            S_i = neighbors[best_sol_index].solution;
            // printf("%d\n", tb_order.queue.size());
            if(neighbors[best_sol_index].kind == 1)
                tb_order.add_tabu_os(S_i.os);
            if(neighbors[best_sol_index].kind == 2)
                for(auto it = neighbors[best_sol_index].tabu_arrange.begin(); it != neighbors[best_sol_index].tabu_arrange.end(); it++)
                    tb_machine.add_tabu_os(*it);
            if(neighbors[best_sol_index].kind == 2)
                for(auto it = neighbors[best_sol_index].tabu_arrange.begin(); it != neighbors[best_sol_index].tabu_arrange.end(); it++)
                    tb_worker.add_tabu_os(*it);
            for(int i = 0; i < neighbors_num; i++)
                if(i != best_sol_index && !in_tabu_list[i])
                    retain_set.add_to_retain(neighbors[i]);
        }
        Sc_i = solution_decode(S_i);
        //printf("TS: %d\n", Sc_i.max_time);
        gen++;
    } while(gen < t_gen);
    return S_i;
}
