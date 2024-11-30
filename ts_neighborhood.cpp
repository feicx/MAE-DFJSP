//
// tabu search neighbourhood struct
//

#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <cstdio>

#include "fjsp.h"
#include "const.h"
#include "ts_neighborhood.h"
#include "ts_tabulist.h"
#include "schedule.h"
#include "solution.h"

struct ts_solution ts_neighborhood_swap(struct solution &S){
    srand(time(NULL));
    struct ts_solution S_n;
    S_n.solution = S;
    S_n.max_time = -1;
    if(S.os.size() == 1)
        return S_n;
    bool *position_used = new bool[S.os.size()];
    int cnt_position_used = 0, s_os_size = S.os.size();
    for(int i = 0; i < s_os_size; i++)
        position_used[i] = false;
    for(int i = 0; i < t_swap; i++){
        int x, y, cnt = 0;
        do{
            cnt++;
            if(cnt > t_break)
                break;
            x = rand() % S.os.size();
        } while(!position_used[x]);
        do{
            cnt++;
            if(cnt > t_break)
                break;
            y = rand() % S.os.size();
        } while(x != y && !position_used[y]);
        if(cnt > t_break)
            continue;
        std::swap(S_n.solution.os[x], S_n.solution.os[y]);
        std::swap(S_n.solution.ms[x], S_n.solution.ms[y]);
        std::swap(S_n.solution.ws[x], S_n.solution.ws[y]);
        position_used[x] = true;
        position_used[y] = true;
        cnt_position_used += 2;
        if(cnt_position_used + 1 <= S.os.size())
            break;
    }
    delete[] position_used;
    schedule Sc_n = solution_decode(S_n.solution);
    S_n.max_time = Sc_n.max_time;
    return S_n;
}

struct ts_solution ts_neighborhood_insert(struct solution &S){
    srand(time(NULL));
    struct ts_solution S_n;
    S_n.solution = S;
    S_n.max_time = -1;
    for(int i = 0; i < t_insert; i++){
        int x = 0, y = 0;
        while(x != y) {
            x = rand() % S.os.size();
            y = rand() % S.os.size();
        }
        if(x > y)
            std::swap(x, y);
        //printf("%d %d\n", x, y);
        int ios = S.os[x], ims = S.ms[x], iws = S.ws[x];
        S.os.erase(S.os.begin() + x);
        S.ms.erase(S.ms.begin() + x);
        S.ws.erase(S.ws.begin() + x);
        S.os.insert(S.os.begin() + y, ios);
        S.ms.insert(S.ms.begin() + y, ims);
        S.ws.insert(S.ws.begin() + y, iws);
    }
    /*for(int i = 0; i < S_n.solution.os.size(); i++)
        printf("%d %d %d\n",S_n.solution.os[i], S_n.solution.ms[i], S_n.solution.ws[i]);*/
    schedule Sc_n = solution_decode(S_n.solution);
    S_n.max_time = Sc_n.max_time;
    return S_n;
}

struct ts_solution ts_neighborhood_machine(struct solution &S){
    srand(time(NULL));
    struct ts_solution S_n;
    S_n.solution = S;
    S_n.max_time = -1;
    struct ts_tabu_resource_node *candidate_machine_inf = new struct ts_tabu_resource_node[S.os.size()];
    int *job_orders = new int[S.os.size()];
    bool fail_find = false;
    int s_os_size = S.os.size();
    for(int i = 0; i < s_os_size; i++)
        job_orders[i] = 0;
    for(int i = 0; i < s_os_size; i++){
    	if(S.os[i] >= num_jobs || S.os[i] < 0)
    		return S_n;
    	if(job_orders[S.os[i]] >= jobs[S.os[i]].num_operations)
    		return S_n;
        candidate_machine_inf[i].resource = jobs[S.os[i]].operations[job_orders[S.os[i]]].candidate_machine.size();
        candidate_machine_inf[i].job_id = S.os[i];
        candidate_machine_inf[i].order = job_orders[S.os[i]];
        job_orders[S.os[i]]++;
    }
    delete[] job_orders;
    //printf("N2 - 1\n");
    for(int i = 0; i < t_machine; i++){
        int x, y, cnt = 0;
        do{
            cnt++;
            if(cnt > t_break)
                break;
            x = rand() % s_os_size;
        } while(candidate_machine_inf[x].resource <= 1);
        if(cnt > t_break){
            if(i == 0)
                fail_find = true;
            break;
        }
        cnt = 0;
        do{
        	cnt++;
            if(cnt > t_break)
                break;
            y = rand() % candidate_machine_inf[x].resource;
        } while(y != S.ms[x]);
        if(y == S.ms[x]){
            if(i == 0)
                fail_find = true;
            break;
        }
        S_n.solution.ms[x] = y;
        ts_tabu_resource_node Tr_n;
        Tr_n = candidate_machine_inf[x];
        Tr_n.resource = y;
        S_n.tabu_arrange.push_back(Tr_n);
    }
    //printf("N2 - 2\n");
    if(fail_find){
        delete[] candidate_machine_inf;
        return S_n;
    }
    schedule Sc_n = solution_decode(S_n.solution);
    S_n.max_time = Sc_n.max_time;
    clear_schedule(Sc_n);
    delete[] candidate_machine_inf;
    return S_n;
}

struct ts_solution ts_neighborhood_worker(struct solution &S){
    srand(time(NULL));
    struct ts_solution S_n;
    S_n.solution = S;
    S_n.max_time = -1;
    struct ts_tabu_resource_node *candidate_worker_inf = new struct ts_tabu_resource_node[S.os.size()];
    int *job_orders = new int[S.os.size()];
    bool fail_find = false;
    int s_os_size = S.os.size();
    for(int i = 0; i < s_os_size; i++)
        job_orders[i] = 0;
    for(int i = 0; i < s_os_size; i++){
    	if(S.os[i] >= num_jobs || S.os[i] < 0)
    		return S_n;
    	if(job_orders[S.os[i]] >= jobs[S.os[i]].num_operations)
    		return S_n;
        candidate_worker_inf[i].resource = jobs[S.os[i]].operations[job_orders[S.os[i]]].candidate_worker.size();
        candidate_worker_inf[i].job_id = S.os[i];
        candidate_worker_inf[i].order = job_orders[S.os[i]];
        job_orders[S.os[i]]++;
    }
    delete[] job_orders;
    //printf("N1 - 1\n");
    for(int i = 0; i < t_worker; i++){
        int x, y, cnt = 0;
        do{
            cnt++;
            if (cnt > t_break)
                break;
            x = rand() % s_os_size;
        } while (candidate_worker_inf[x].resource <= 1);
        if (cnt > t_break){
            if (i == 0)
                fail_find = true;
            break;
        }
        cnt = 0;
        do {
        	cnt++;
        	if (cnt > t_break)
                break;
            y = rand() % candidate_worker_inf[x].resource;
        } while (y != S.ws[x]);
        if (y == S.ws[x]){
            if (i == 0)
                fail_find = true;
            break;
        }
        S_n.solution.ws[x] = y;
        ts_tabu_resource_node Tr_n;
        Tr_n = candidate_worker_inf[x];
        Tr_n.resource = y;
        S_n.tabu_arrange.push_back(Tr_n);
    }
    //printf("N1 - 2\n");
    if(fail_find){
        delete[] candidate_worker_inf;
        S_n.max_time = -1;
        return S_n;
    }
    schedule Sc_n = solution_decode(S_n.solution);
    S_n.max_time = Sc_n.max_time;
    delete[] candidate_worker_inf;
    clear_schedule(Sc_n);
    return S_n;
}
