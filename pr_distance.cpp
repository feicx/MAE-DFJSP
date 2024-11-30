//
// Calculate the distance in PR
//

#include <utility>
#include <map>
#include <cmath>
#include <cstdio>

#include "fjsp.h"
#include "pr_distance.h"
#include "schedule.h"
#include "solution.h"

int pr_distance(const schedule &S1, const schedule &S2){
    int distance = 0;
    for(std::map<std::pair<int, int>, struct schedule_node>::const_iterator it = S1.schedule.begin(); it!=S1.schedule.end(); it++){
        int Location_S1 = (it->second.begin_time + it->second.end_time) / 2;
        struct schedule_node o_S2 = S2.schedule.find(std::make_pair(it->second.job_id, it->second.jon_order))->second;
        int Location_S2 = (o_S2.begin_time + o_S2.end_time) / 2;
        //printf("Dis: %d %d %d %d\n", Location_S1, Location_S2, it->second.machine_id, o_S2.machine_id, it->second.worker_id, o_S2.worker_id);
        if(it->second.machine_id == o_S2.machine_id)
            distance = distance + abs(Location_S1 - Location_S2);
        else{
        	int LS1, LS2;
        	LS1 = LS2 = decode_max_ans + 666;
        	if(it->second.machine_id < num_machines && it->second.machine_id >= 0)
            	LS1 = S1.machine_L[it->second.machine_id];
            if(o_S2.machine_id < num_machines && o_S2.machine_id >= 0)
				LS2 = S2.machine_L[o_S2.machine_id];
            distance = distance + std::min(Location_S1 + Location_S2, LS1 + LS2 - Location_S1 - Location_S2);
        }
        if(it->second.worker_id == o_S2.worker_id)
            distance = distance + abs(Location_S1 - Location_S2);
        else{
        	int LS1, LS2;
        	LS1 = LS2 = decode_max_ans + 666;
        	if(it->second.machine_id < num_machines)
            	LS1 = S1.worker_L[it->second.worker_id];
            if(o_S2.machine_id < num_workers)
            	LS2 = S2.worker_L[o_S2.worker_id];
            //printf("  W: %d %d\n",LS1, LS2);
            distance = distance + std::min(Location_S1 + Location_S2, LS1 + LS2 - Location_S1 - Location_S2);
        }
    }
    return distance;
}

int pr_distance(const schedule &S1, const schedule &S2, std::pair<int, int> operation){
    //printf("%d %d\n", operation.first, operation.second);
    int distance = 0;
    struct schedule_node o_S1 = S1.schedule.find(operation)->second;
    int Location_S1 = (o_S1.begin_time + o_S1.end_time) / 2;
    struct schedule_node o_S2 = S2.schedule.find(operation)->second;
    int Location_S2 = (o_S2.begin_time + o_S2.end_time) / 2;
    if(o_S1.machine_id == o_S2.machine_id)
        distance = distance + abs(Location_S1 - Location_S2);
    else{
    	int LS1, LS2;
        LS1 = LS2 = decode_max_ans + 666;
        if(o_S1.machine_id < num_machines && o_S1.machine_id >= 0)
        	LS1 = S1.machine_L[o_S1.machine_id];
        if(o_S2.machine_id < num_machines && o_S2.machine_id >= 0)
        	LS2 = S2.machine_L[o_S2.machine_id];
        distance = distance + std::min(Location_S1 + Location_S2, LS1 + LS2 - Location_S1 - Location_S2);
    }
    if(o_S1.worker_id == o_S2.worker_id)
        distance = distance + abs(Location_S1 - Location_S2);
    else{
    	int LS1, LS2;
        LS1 = LS2 = decode_max_ans + 666;
        if(o_S1.worker_id < num_workers && o_S1.worker_id >= 0)
        	LS1 = S1.worker_L[o_S1.worker_id];
        if(o_S2.worker_id < num_workers && o_S2.worker_id >= 0)
        	LS2 = S2.worker_L[o_S2.worker_id];
        //printf("  W: %d %d\n",LS1, LS2);
        distance = distance + std::min(Location_S1 + Location_S2, LS1 + LS2 - Location_S1 - Location_S2);
    }
    return distance;
}
