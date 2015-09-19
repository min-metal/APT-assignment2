/***********************************************************************
 * COSC1076 - Advanced Programming Techniques
 * Semester 2 2015 Assignment #2
 * Full Name        : EDIT HERE
 * Student Number   : EDIT HERE
 * Course Code      : EDIT HERE
 * Program Code     : EDIT HERE
 * Start up code provided by Paul Miller
 * Some codes are adopted here with permission by an anonymous author
 ***********************************************************************/
#include "ppd_stock.h"
#include "ppd_utility.h"

 /**
  * @file ppd_stock.c this is the file where you will implement the
  * interface functions for managing the stock list.
  **/

int add_to_list(struct ppd_list * list, struct ppd_stock * data)
{
    struct ppd_node * new = safe_malloc(sizeof(*new));
    new->data = data;
    new->next = NULL;

    struct ppd_node * previous = NULL;
    struct ppd_node * current = list->head;

    while(current != NULL)
    {
        if(strcmp(data->id, current->data->id) < 0)
            break;

        previous = current;
        current = current->next;
    }

    if(list->head == NULL)
    {
        list->head = new;
    }
    else if (previous == NULL)
    {
        new->next = list->head;
        list->head = new;
    }
    else
    {
        previous->next = new;
        new->next = current;
    }
    list->count++;

    return 1;

}

