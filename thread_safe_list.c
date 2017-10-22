#include "thread_safe_list.h"
#include <stdlib.h>

int init_list(thread_safe_list* list){
    list->head = NULL;
    pthread_mutex_init(&list->mutex_on_list,NULL);
    return 0;
}

char tos(type t){
    if(t==REDACTEUR)
        return 'R';
    else
        return 'L';
}
//Pre-condition : list is not empty
//If the first element is the one we are looking for : remove it and return true
//Else list is not modified and return false
int is_head(thread_safe_list* list,type type, int thread_id){

    pthread_mutex_lock(&list->mutex_on_list);
    int ret = 0;
        if(list->head->id->thread_id == thread_id){
            ret = 1;
            list->head = list->head->next;
            if(list->head == NULL){
                list->tail = NULL;
            }
        }
    pthread_mutex_unlock(&list->mutex_on_list);
    return ret;
}

//Add element at the end of the list
void add_element(thread_safe_list* list,type type, int thread_id){

    pthread_mutex_lock(&list->mutex_on_list);
    node* node =  malloc(sizeof(node));
    identifier_t* identifier = malloc(sizeof(identifier_t));
    identifier->thread_id = thread_id;
    identifier->type = type;
    node->id = identifier;
    node->next=NULL;
    if (list->head == NULL){
        list->head = node;
        list->tail = node;
    }
    else{
        list->tail->next = node;
        list->tail = node;
    }
    pthread_mutex_unlock(&list->mutex_on_list);
}

//Return the type of the first element
type head_element_type(thread_safe_list* list){

    pthread_mutex_lock(&list->mutex_on_list);
    type ret;
    if(list->head == NULL){
        ret = NO_TYPE;
    }
    else{
        ret = list->head->id->type;
    }
    pthread_mutex_unlock(&list->mutex_on_list);
    return ret;
}
