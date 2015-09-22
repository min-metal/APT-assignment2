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
    struct ppd_node * previous = NULL;
    struct ppd_node * current = list->head;

    struct ppd_node * new = safe_malloc(sizeof(*new));
    new->data = data;
    new->next = NULL;

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
        new->next = current;
        previous->next = new;
    }
    list->count++;

    return 1;

}

int load_stock(struct ppd_list * list, char * string)
{
    int number_of_lines = 0, i, j, str_len;
    int successful_read = 0;
    char * start = NULL;
    char temp[NO_ATTRIBUTE][DESCLEN + EXTRA_SPACE];


    number_of_lines = explode_input(string, '\n') - 1;


    printf("\nNo. lines to read: %i\n", number_of_lines);

/*    for(i = 0; i < 70; ++i)
        printf("%i, ", (int) *(string + i));*/

    start = string;
    while(*start != '\0')
    {
        if (count_delim_in_string(start, STOCK_DELIM) != NO_DELIMS){
            printf("Wrong number of delimiters!\n");
            return FAILURE;
        }

        explode_input(start, STOCK_DELIM);

        for(j = 0; j < NO_ATTRIBUTE; ++j) {
            if ((str_len = strlen(start)) > DESCLEN)
                return FAILURE;

            strcpy(temp[j], start);
            printf("%s\n", start);
            start += str_len + 1;
        }

        /* create new stock and add to list */
        if(new_stock(temp) == NULL) /*TODO free */
        {
            return FAILURE;
        }


    }

    return SUCCESS;

}

struct ppd_stock * new_stock(char attributes[][DESCLEN + EXTRA_SPACE])
{
    struct ppd_stock * stock = safe_malloc(sizeof(*stock));

    if (strlen(attributes[0]) > IDLEN)
    {
        fprintf(stderr, "ID must be < %i characters.\n", IDLEN);
        free(stock);
        return NULL;
    }
    if (strlen(attributes[1]) > NAMELEN)
    {
        fprintf(stderr, "Name must be < %i characters.\n", NAMELEN);
        free(stock);
        return NULL;
    }
    if (strlen(attributes[2]) > DESCLEN)
    {
        fprintf(stderr, "Description must be < %i characters.\n", DESCLEN);
        free(stock);
        return NULL;
    }
    if (strlen(attributes[3]) > PRICELEN)
    {
        fprintf(stderr, "Price must be < %i characters.\n", PRICELEN);
        free(stock);
        return NULL;
    }
    if (strlen(attributes[4]) > ONHANDLEN)
    {
        fprintf(stderr, "Stock level must be < %i characters.\n", ONHANDLEN);
        free(stock);
        return NULL;
    }

    return stock;
}

int count_delim_in_string(const void * str, int c)
{
    int count = 0;
    const char * p = str;

    while((p = strchr(p, c)) != NULL)
    {
        ++count;
        ++p; /* increment pointer to char after delim */
    }

    return count;
}

