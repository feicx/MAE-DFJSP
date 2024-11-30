//
// Linked List
//

#include "list.h"
#include "solution.h"

#include <iostream>

list_node::list_node() {
    this->next = this->last = NULL;
}

list_node* list_insert_behind(list_node *position, time_interval &interval){
	list_node *p;
    try{
    	p = new list_node;
	} catch (const std::bad_alloc& e){
    	std::cerr << "Memory Error:" << e.what() << std::endl;
        return NULL;
	}
    p->time_interval.begin_time = interval.begin_time;
    p->time_interval.end_time = interval.end_time;
    p->next = NULL;
    p->last = NULL;
    if(position == NULL)
        return p;
    p->next = position->next;
    p->last = position;
    position->next = p;
    if(p->next != NULL)
        p->next->last = p;
    return p;
}

list_node* list_insert_front(list_node *position, time_interval &interval){
    list_node *p;
    try{
        p = new list_node;
    } catch (const std::bad_alloc& e){
        std::cerr << "Memory Error:" << e.what() << std::endl;
        return NULL;
    }
    p->time_interval.begin_time = interval.begin_time;
    p->time_interval.end_time = interval.end_time;
    p->next = NULL;
    p->last = NULL;
    if(position == NULL)
        return p;
    p->next = position;
    p->last = position -> last;
    if(p->last != NULL)
        p->last->next = p;
    position->last = p;
    return p;
}

void list_free(list_node *head){
    if(head == NULL)
        return;
    list_node *p;
    for(p = head; p->next != NULL; p = p->next)
        if(p->last != NULL)
            delete p->last;
    delete p;
}
