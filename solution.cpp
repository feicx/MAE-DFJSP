//
// Random Solution Initialization
// Solution Decode
//

#include <algorithm>
#include <set>

#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <cmath>

#include "fjsp.h"
#include "schedule.h"
#include "solution.h"
#include "list.h"

void clear_schedule(struct schedule Sc){
    if(Sc.machine_L == NULL)
        delete[] Sc.worker_L;
    if(Sc.worker_L == NULL)
        delete[] Sc.machine_L;
}

struct solution& solution::operator=(const solution &S){
    this->os = S.os;
    this->ms = S.ms;
    this->ws = S.ws;
    return *this;
}

int decode_max_ans = -1;

bool solution_verify(struct solution &S){
	int *verify_os = new int[num_jobs];
    for(int i = 0; i < num_jobs; i++)
        verify_os[i] = 0;
    int s_os_size = S.os.size();
    for(int i = 0; i < s_os_size; i++){
    	if(S.os[i] >= num_jobs || S.os[i] < 0){
            delete[] verify_os;
			return false;
		}
        if(verify_os[S.os[i]] >= jobs[S.os[i]].num_operations){
            delete[] verify_os;
			return false;
		}
        if(S.ms[i] >= jobs[S.os[i]].operations[verify_os[S.os[i]]].candidate_machine.size()){
            delete[] verify_os;
			return false;
		}
		if(S.ws[i] >= jobs[S.os[i]].operations[verify_os[S.os[i]]].candidate_worker.size()){
            delete[] verify_os;
			return false;
		}
		verify_os[S.os[i]]++;
    }
    delete[] verify_os;
    return true; 
}

// decoding of greedy insertion
schedule solution_decode(struct solution &S, bool out_debug){
    schedule sch;
    sch.max_time = 0;
    int s_os_size = S.os.size();
    int *ope = new int[s_os_size], *time = new int[s_os_size], *ms = new int[s_os_size], *ws = new int[s_os_size];
    int *verify_os = new int[num_jobs];
    for(int i = 0; i < num_jobs; i++)
        verify_os[i] = 0;
    for(int i = 0; i < s_os_size; i++){
        if(S.os[i] >= num_jobs || S.os[i] < 0){
            delete[] ope;
            delete[] time;
            delete[] ms;
            delete[] ws;
            delete[] verify_os;
            sch.max_time = -1;
            return sch;
        }
        if(verify_os[S.os[i]] >= jobs[S.os[i]].num_operations){
            delete[] ope;
            delete[] time;
            delete[] ms;
            delete[] ws;
            delete[] verify_os;
            sch.max_time = -1;
            return sch;
        }
        ope[i] = verify_os[S.os[i]];
        time[i] = jobs[S.os[i]].operations[verify_os[S.os[i]]].time;
        if(S.ms[i] < 0 || S.ms[i] >= jobs[S.os[i]].operations[verify_os[S.os[i]]].candidate_machine.size()){
            delete[] ope;
            delete[] time;
            delete[] ms;
            delete[] ws;
            delete[] verify_os;
            sch.max_time = -1;
            return sch;
        }
        ms[i] = jobs[S.os[i]].operations[verify_os[S.os[i]]].candidate_machine[S.ms[i]].id;
        time[i] = std::round(jobs[S.os[i]].operations[verify_os[S.os[i]]].candidate_machine[S.ms[i]].factor * time[i] + 0.5);
        if(S.ws[i] < 0 || S.ws[i] >= jobs[S.os[i]].operations[verify_os[S.os[i]]].candidate_worker.size()){
            delete[] ope;
            delete[] time;
            delete[] ms;
            delete[] ws;
            delete[] verify_os;
            sch.max_time = -1;
            return sch;
        }
        ws[i] = jobs[S.os[i]].operations[verify_os[S.os[i]]].candidate_worker[S.ws[i]].id;
        time[i] = std::round(jobs[S.os[i]].operations[verify_os[S.os[i]]].candidate_worker[S.ws[i]].factor * time[i] + 0.5);
        verify_os[S.os[i]]++;
    }
    delete[] verify_os;
    sch.machine_L = new int[num_machines + 5];
    sch.worker_L = new int[num_workers + 5];
    for(int i = 0; i < num_machines + 5; i++)
        sch.machine_L[i] = 0;
    for(int i = 0; i < num_workers + 5; i++)
        sch.worker_L[i] = 0;
    struct list_node **interval_m = new list_node*[num_machines];
    struct list_node **interval_w = new list_node*[num_workers];
    int *job_L = new int[num_jobs];
    time_interval zero_time;
    zero_time.begin_time = 0;
    zero_time.end_time = 0;
    for(int i = 0; i < num_jobs; i++)
        job_L[i] = 0;
    for(int i = 0; i < num_machines; i++)
        interval_m[i] = list_insert_behind(NULL, zero_time);
    for(int i = 0; i < num_workers; i++)
        interval_w[i] = list_insert_behind(NULL, zero_time);
    for(int i = 0; i < s_os_size; i++){
        struct list_node *p_m = interval_m[ms[i]];
        struct list_node *p_w = interval_w[ws[i]];
        struct time_interval interval;
        interval.begin_time = job_L[S.os[i]];
        if(p_m->next != NULL && p_w->next == NULL){
            if(out_debug)
                printf("Con 0M: %d\n", i);
            while(p_m->next != NULL && p_m->next->time_interval.begin_time <= interval.begin_time)
                p_m = p_m->next;
            while(p_m->next != NULL && p_m->next->time_interval.begin_time - std::max(p_m->time_interval.end_time, interval.begin_time) < time[i])
                p_m = p_m->next;
        }
        else if(p_m->next == NULL && p_w->next != NULL){
            if(out_debug)
                printf("Con 0W: %d\n", i);
            while(p_w->next != NULL && p_w->next->time_interval.begin_time <= interval.begin_time)
                p_w = p_w->next;
            while(p_w->next != NULL && p_w->next->time_interval.begin_time - std::max(p_w->time_interval.end_time, interval.begin_time) < time[i])
                p_w = p_w->next;
        }
        else{
            if(out_debug)
                printf("Con Else: %d\n", i);
            while(p_m->next != NULL || p_w->next != NULL){
                while(p_m->next != NULL && p_m->next->time_interval.begin_time <= interval.begin_time)
                    p_m = p_m->next;
                while(p_w->next != NULL && p_w->next->time_interval.begin_time <= interval.begin_time)
                    p_w = p_w->next;
                bool flag = true;
                while(flag){
                    flag = false;
                    while(p_m->next != NULL && p_m->next->time_interval.begin_time <= p_w->time_interval.end_time){
                        p_m = p_m->next;
                        flag = true;
                    }
                    while(p_w->next != NULL && p_w->next->time_interval.begin_time <= p_m->time_interval.end_time){
                        p_w = p_w->next;
                        flag = true;
                    }
                }
                int arrange_time = std::max(interval.begin_time, std::max(p_m->time_interval.end_time, p_w->time_interval.end_time));
                if(p_m->next == NULL && p_w->next == NULL){
                    if(out_debug)
                        printf("Con0 - %d\n", i);
                    interval.begin_time = arrange_time;
                    break;
                }
                else if(p_m->next == NULL){
                    if(p_w->next->time_interval.begin_time - arrange_time >= time[i]){
                        if(out_debug)
                            printf("Con1: %d\n", i);
                        interval.begin_time = arrange_time;
                        break;
                    }
                }
                else if(p_w->next == NULL){
                    if(p_m->next->time_interval.begin_time - arrange_time >= time[i]){
                        if(out_debug)
                            printf("Con2: %d\n", p_m->next->time_interval.begin_time - arrange_time >= time[i]);
                        interval.begin_time = arrange_time;
                        break;
                    }
                }
                else if(std::min(p_w->next->time_interval.begin_time, p_m->next->time_interval.begin_time) - arrange_time >= time[i]){
                    if(out_debug)
                        printf("Con3: %d\n", std::min(p_w->next->time_interval.begin_time, p_m->next->time_interval.begin_time) - arrange_time);
                    interval.begin_time = arrange_time;
                    break;
                }
                if(p_m->next != NULL)
                    p_m = p_m->next;
                if(p_w->next != NULL)
                    p_w = p_w->next;
            }
        }
        interval.begin_time = std::max(interval.begin_time, std::max(p_m->time_interval.end_time, p_w->time_interval.end_time));
        if(p_m->next == NULL && p_w->next == NULL && out_debug)
            printf("Con C: %d\n", i);
        interval.end_time = interval.begin_time + time[i];
        if(out_debug){
            printf("M: %d %d %d", ms[i], interval.begin_time, interval.end_time);
            if(p_m->next != NULL)
                printf(" %d %d\n", p_m->next->time_interval.begin_time, p_m->next->time_interval.end_time);
            else
                printf("\n");
            printf("W: %d %d %d", ws[i], interval.begin_time, interval.end_time);
            if(p_w->next != NULL)
                printf(" %d %d\n", p_w->next->time_interval.begin_time, p_w->next->time_interval.end_time);
            else
                printf("\n");
        }
        list_insert_behind(p_m, interval);
        list_insert_behind(p_w, interval);
        if(out_debug){
            printf("M: ");
            for(struct list_node *p = interval_m[ms[i]];p;p=p->next)
                printf("%d %d ", p->time_interval.begin_time, p->time_interval.end_time);
            printf("\nW: ");
            for(struct list_node *p = interval_w[ws[i]];p;p=p->next)
                printf("%d %d ", p->time_interval.begin_time, p->time_interval.end_time);
            printf("\n");
        }
        struct schedule_node sch_o;
        sch_o.begin_time = interval.begin_time;
        sch_o.end_time = interval.end_time;
        sch_o.job_id = S.os[i];
        sch_o.jon_order = ope[i];
        sch_o.worker_id = ws[i];
        sch_o.machine_id = ms[i];
        job_L[S.os[i]] = sch_o.end_time;
        sch.schedule[std::make_pair(sch_o.job_id, sch_o.jon_order)] = sch_o;
        sch.max_time = std::max(sch.max_time, sch_o.end_time);
    }
    for(int i = 0; i < num_machines; i++)
        list_free(interval_m[i]);
    for(int i = 0; i < num_workers; i++)
        list_free(interval_w[i]);
    delete[] interval_m;
    delete[] interval_w;
    delete[] job_L;
    delete[] ope;
    delete[] time;
    delete[] ms;
    delete[] ws;
    decode_max_ans = std::max(decode_max_ans, sch.max_time);
    return sch;
}

// to init a random solution.
struct solution solution_init(){
    int *job_operations_num = new int[num_jobs];
    int select_job = num_jobs;
    for(int i = 0; i < num_jobs; i++)
        job_operations_num[i] = 0;
    solution sol;
    while(select_job > 0){
        int os = 0;
        if(select_job > 1)
            os = rand() % select_job;
        int c_os = os + 1;
        for(int i = 0; i < num_jobs && c_os > 0; i++)
            if(job_operations_num[i] >= jobs[i].num_operations)
                os++;
            else
                c_os--;
        int ms = rand() % jobs[os].operations[job_operations_num[os]].candidate_machine.size();
        int ws = rand() % jobs[os].operations[job_operations_num[os]].candidate_worker.size();
        sol.os.push_back(os);
        sol.ms.push_back(ms);
        sol.ws.push_back(ws);
        job_operations_num[os]++;
        if(job_operations_num[os] >= jobs[os].num_operations)
            select_job--;
    }
    delete[] job_operations_num;
    return sol;
}
