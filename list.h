//
// Linked List
//

#ifndef MAE_LIST_H
#define MAE_LIST_H

#include "solution.h"

struct list_node{
    struct time_interval time_interval;
    struct list_node *last, *next;
    list_node();
};

list_node* list_insert_front(list_node *position, time_interval &interval);
list_node* list_insert_behind(list_node *position, time_interval &interval);
void list_free(list_node *head);

#endif //MAE_LIST_H
