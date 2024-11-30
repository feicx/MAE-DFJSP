//
// Tabu Search Tabu List
//

#ifndef CODE_TS_TABULIST_H
#define CODE_TS_TABULIST_H

#include <list>
#include <vector>

struct ts_tabulist{
    int max_size;
    ts_tabulist();
};

struct ts_tabulist_order : ts_tabulist{
    std::list<std::vector<int> > queue;
    bool in_tabulist(std::vector<int> os);
    void add_tabu_os(std::vector<int> os);
};

struct ts_tabu_resource_node{
    int job_id;
    int order;
    int resource;
    ts_tabu_resource_node(int job_id, int order, int resource);
    ts_tabu_resource_node();
    bool operator==(const struct ts_tabu_resource_node &p) const;
};

struct ts_tabulist_resource : ts_tabulist{
    std::list<struct ts_tabu_resource_node> queue;
    bool in_tabulist(const ts_tabu_resource_node &p);
    void add_tabu_os(const ts_tabu_resource_node &p);
};

#endif //CODE_TS_TABULIST_H
