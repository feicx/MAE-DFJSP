#include <iostream>
#include <fstream>
#include <chrono>
#include <map>

#include <cstdlib>
#include <ctime>

#include "const.h"
#include "fjsp.h"
#include "schedule.h"
#include "solution.h"
#include "pathrelinking.h"
#include "pr_distance.h"
#include "tabu_search.h"

int num_jobs, num_machines, num_workers;
std::vector<job_node> jobs;

solution save_best(solution &S1, solution &S2){
    schedule Sc_1 = solution_decode(S1);
    schedule Sc_2 = solution_decode(S2);
    clear_schedule(Sc_1);
    clear_schedule(Sc_2);
    if(Sc_1.max_time < Sc_2.max_time)
        return S1;
    return S2;
}

bool similar(solution &S1, solution &S2){
    schedule Sc_1 = solution_decode(S1);
    schedule Sc_2 = solution_decode(S2);
    clear_schedule(Sc_1);
    clear_schedule(Sc_2);
    if(pr_distance(Sc_1, Sc_2) < decode_max_ans / 20 + 5)
        return true;
    return false;
}


solution mae(){
    std::ofstream fout("log.txt");
    int last_sc_time = 0, last_gen, last_cycle, last_time;
    int gen = 0, cycle = 1;
    solution S1, S2, S_C, S_P, S;
    S1 = solution_init();
    S2 = solution_init();
    S_C = solution_init();
    S_P = solution_init();
    S = solution_init();
    auto start = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = std::chrono::high_resolution_clock::now() - start;
    while(elapsed.count() < max_time){
        gen++;
        solution S1_pr = path_relinking(S1, S2);
        solution S2_pr = path_relinking(S2, S1);
        S1 = tabu_search(S1_pr);
        S2 = tabu_search(S2_pr);
        solution S_12 = save_best(S1, S2);
        S_C = save_best(S_C, S_12);
        S = save_best(S_C, S);
        schedule Sc = solution_decode(S);
        clear_schedule(Sc);
        std::cout << "Cycle "<<cycle<<", Gen "<<gen<<", Best Estimate Time: "<<Sc.max_time<<std::endl;
        fout << "Cycle "<<cycle<<", Gen "<<gen<<", Best Estimate Time: "<<Sc.max_time<<std::endl;
        if(last_sc_time != Sc.max_time){
            elapsed = std::chrono::high_resolution_clock::now() - start;
            last_cycle = cycle;
            last_gen = gen;
            last_time = elapsed.count();
            last_sc_time = Sc.max_time;
        }
        if(gen == P){
            cycle++;
            std::cout<<"Cycle "<<cycle<<std::endl;
            S1 = S_P;
            S_P = S_C;
            S_C = solution_init();
            gen = 0;
        }
        if(similar(S1, S2))
            S2 = solution_init();
        elapsed = std::chrono::high_resolution_clock::now() - start;
    }
    fout<<"Earliest Best Solution Cycle: "<<last_cycle<<", Gen: "<<last_gen<<", Time: "<<last_time<<"s"<<std::endl;
    std::cout <<"---- Maximum Time Limit Reached ----" <<std::endl;
    return S;
}

int main() {
    srand(time(NULL));
    std::cout << "---------- MAE Algorithm ----------" << std::endl;
    std::ifstream fin("input.txt");
    fin>>num_jobs>>num_machines>>num_workers;
    for(int i = 0; i < num_jobs; i++){
        job_node job;
        job.id = i;
        fin >> job.num_operations;
        for(int j = 0; j < job.num_operations; j++){
            operation_node operation;
            operation.order = j;
            int T;
            fin >> operation.time >> T;
            while(T--){
                efficient_node efficient;
                fin >> efficient.id;
                efficient.id--;
                fin >> efficient.factor;
                operation.candidate_machine.push_back(efficient);
            }
            fin >> T;
            while(T--){
                efficient_node efficient;
                fin >> efficient.id;
                efficient.id--;
                fin >> efficient.factor;
                operation.candidate_worker.push_back(efficient);
            }
            job.operations.push_back(operation);
        }
        jobs.push_back(job);
    }
    solution S = mae();
    schedule S_c = solution_decode(S);
    std::ofstream fout("solution.csv");
    std::cout << "Schedule Planning (Estimate Time: " << S_c.max_time << ")"<< std::endl;
    fout << "Job, Operation, Begin Time, End Time, Machine, Worker"<< std::endl;
    for(auto it = S_c.schedule.begin(); it != S_c.schedule.end(); it++){
        std::cout <<"Job: "<< it->second.job_id + 1 << "\t Operation: " << it->second.jon_order + 1 << "\t Begin Time: " << it->second.begin_time;
        std::cout  << "\t End Time: " << it->second.end_time << "\t Machine: " << it->second.machine_id + 1 << "\t Worker: " << it->second.worker_id + 1 << std::endl;
        fout << it->second.job_id + 1 << "," << it->second.jon_order + 1 << "," << it->second.begin_time << "," << it->second.end_time;
        fout << "," << it->second.machine_id + 1 << "," << it->second.worker_id + 1 << std::endl;
    }
    std::cout << "Successfully..." << std::endl;
    return 0;
}