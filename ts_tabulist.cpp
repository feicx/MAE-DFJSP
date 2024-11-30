//
// Tabu List: Job Schedule, Machine and Worker Arrange
//

#include <list>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cstdio>

#include "fjsp.h"
#include "ts_tabulist.h"

//dynamic tabu list build
ts_tabulist::ts_tabulist(){
    srand(time(NULL));
    int L = 10 + std::min(num_jobs / num_machines, num_jobs / num_workers);
    if(num_jobs <= 2 * num_machines && num_jobs <= 2 * num_workers)
        this->max_size = rand() % (L * 2 / 5) + L;
    else if(num_jobs <= 2 * num_machines || num_jobs <= 2 * num_workers)
        this->max_size = rand() % (L * 9 / 20) + L;
    else{
        this->max_size = rand() % (L / 2) + L;
    }
}

void ts_tabulist_order::add_tabu_os(std::vector<int> os){
    if(this->queue.size() >= this->max_size)
        this->queue.pop_front();
    queue.push_front(os);
}

bool ts_tabulist_order::in_tabulist(std::vector<int> os){
    for(std::list<std::vector<int>>::iterator it = this->queue.begin(); it != this->queue.end(); it++){
        if(os == *it)
            return true;
    }
    return false;
}

ts_tabu_resource_node::ts_tabu_resource_node(int job_id, int order, int resource) : job_id(job_id), order(order), resource(resource) {}
ts_tabu_resource_node::ts_tabu_resource_node() {}

bool ts_tabu_resource_node::operator==(const struct ts_tabu_resource_node &p) const{
    if(p.resource == this->resource && p.job_id == this->job_id && p.order == this->order)
        return true;
    return false;
}

void ts_tabulist_resource::add_tabu_os(const ts_tabu_resource_node &p){
    if(this->queue.size() >= this->max_size)
        this->queue.pop_front();
    queue.push_front(p);
}

bool ts_tabulist_resource::in_tabulist(const ts_tabu_resource_node &p){
    for(std::list<struct ts_tabu_resource_node>::iterator it = this->queue.begin(); it != this->queue.end(); it++){
        if(p == *it)
            return true;
    }
    return false;
}