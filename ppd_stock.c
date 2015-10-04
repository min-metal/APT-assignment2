/***********************************************************************
 * COSC1076 - Advanced Programming Techniques
 * Semester 2 2015 Assignment #2
 * Full Name        : Tuan Minh Tran
 * Student Number   : s3488978
 * Course Code      : COSC1076
 * Program Code     : BP094
 * Start up code provided by Paul Miller
 * Some codes are adopted here with permission by an anonymous author
 ***********************************************************************/
/*#include "ppd_stock.h"*/
#include "ppd_utility.h"

 /**
  * @file ppd_stock.c this is the file where you will implement the
  * interface functions for managing the stock list.
  **/

BOOLEAN add_to_list(struct ppd_list * list, struct ppd_stock * data)
{
    struct ppd_node * previous = NULL;
    struct ppd_node * current = list->head;
    struct ppd_node * new;

    if(check_stock_id_in_system(list, data))
    {
        fprintf(stderr, "Duplicate ID!\n");
        return FALSE;
    }

    new = safe_malloc(sizeof(*new));
    new->data = data;
    new->next = NULL;

    while(current != NULL)
    {
        if(strcmp(data->name, current->data->name) < 0)
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

    return TRUE;
}

BOOLEAN remove_from_list(struct ppd_list * list, struct ppd_stock * data)
{
    struct ppd_node * current, * previous;
    previous = NULL;
    current = list->head;

    while(current != NULL)
    {
        if(current->data == data)
            break;
        previous = current;
        current = current->next;
    }
    if(current == NULL)
        return FALSE;

    if(previous == NULL)
    {
        list->head = current->next;
    }
    else
    {
        previous->next = current->next;
    }

    free(current->data);
    free(current);

    --list->count;
    return TRUE;
}

BOOLEAN display_list(struct ppd_list * list)
{
    struct ppd_node * current;
    current = list->head;

    printf("\nItems Menu\n");
    printf("%-6s|%-40s|%-10s|%-6s\n", "ID", "Name", "Available", "Price");
    printf("-----------------------------------------------------------------\n");
    while(current != NULL)
    {
        printf("%-6s|%-40s|%-10u|$%5.2f\n",
               current->data->id, current->data->name, current->data->on_hand,
               (float) (current->data->price.dollars * DOLLAR_TO_CENT
                + current->data->price.cents) / DOLLAR_TO_CENT);

        current = current->next;
    }
    return TRUE;
}

BOOLEAN load_stock(struct ppd_list * list, char * string)
{
    int number_of_lines = 0, j, successful_read = 0;
    size_t str_len;
    char * start = NULL;
    char temp[NO_ATTRIBUTE][DESCLEN + EXTRA_SPACE];
    struct ppd_stock * temp_stock;


    number_of_lines = explode_input(string, '\n') - 1;
    printf("\nNo. lines to read from stock: %i\n", number_of_lines);

    start = string;
    while(*start != '\0')
    {
        if (count_delim_in_string(start, STOCK_DELIM) != NO_STOCK_DELIMS)
        {
            fprintf(stderr, "Wrong number of delimiters! Must have %i '%c'.\n"
                    , NO_STOCK_DELIMS, STOCK_DELIM);
            printf("Each data entry must end in '\\n', with no empty lines.\n");
            return FALSE;
        }

        explode_input(start, STOCK_DELIM);

        for(j = 0; j < NO_ATTRIBUTE; ++j)
        {
            if ((str_len = strlen(start)) > DESCLEN)
            {
                fprintf(stderr, "One or more field contains too many characters\n");
                return FALSE;
            }
            strcpy(temp[j], start);
            start += str_len + 1;
        }

        /* create new stock and add to list */
        if((temp_stock = new_stock(temp)) == NULL)
        {
            return FALSE;
        }
        if(add_to_list(list, temp_stock) == FALSE)
        {
            free(temp_stock);
            return FALSE;
        }

        ++successful_read;
    }

    if(successful_read != number_of_lines)
    {
        fprintf(stderr, "Corrupt data file!\n");
        printf("Each data entry must end in '\\n', with no empty lines.\n");
        return FALSE;
    }


    printf("Load successful\n");
    return TRUE;
}

struct ppd_stock * get_stock_by_id(struct ppd_list * list, char * stock_id)
{
    struct ppd_node * current;
    current = list->head;

    while(current != NULL)
    {
        if(strcmp(current->data->id, stock_id) == 0)
        {
            return current->data;
        }
        current = current->next;
    }
    return NULL;

}

struct ppd_stock * new_stock(char attributes[][DESCLEN + EXTRA_SPACE])
{
    unsigned price, on_hand;
    struct ppd_stock * stock = safe_malloc(sizeof(*stock));

    if (strlen(attributes[ID]) != IDLEN)
    {
        fprintf(stderr, "ID must be %i characters.\n", IDLEN);
        free(stock);
        return NULL;
    }
    if (strlen(attributes[NAME]) > NAMELEN)
    {
        fprintf(stderr, "Name must be < %i characters.\n", NAMELEN);
        free(stock);
        return NULL;
    }
    if (strlen(attributes[DESC]) > DESCLEN)
    {
        fprintf(stderr, "Description must be < %i characters.\n", DESCLEN);
        free(stock);
        return NULL;
    }
    if (strlen(attributes[PRICE]) > PRICELEN)
    {
        fprintf(stderr, "Price must be < %i characters.\n", PRICELEN);
        free(stock);
        return NULL;
    }
    if (strlen(attributes[ONHAND]) > ONHANDLEN)
    {
        fprintf(stderr, "Stock level must be < %i characters.\n", ONHANDLEN);
        free(stock);
        return NULL;
    }

    if (check_stock_id_regex(attributes[ID]) == FALSE)
    {
        fprintf(stderr, "Stock ID mismatch\n");
        free(stock);
        return NULL;
    }
    if (check_price(attributes[PRICE]) == FALSE)
    {
        free(stock);
        return NULL;
    }
    if (check_on_hand(attributes[ONHAND]) == FALSE)
    {
        free(stock);
        return NULL;
    }



    strcpy(stock->id, attributes[ID]);
    strcpy(stock->name, attributes[NAME]);
    strcpy(stock->desc, attributes[DESC]);

    if(((price = get_price(attributes[PRICE])) % CENT_ROUNDING != 0) ||
            (price <= 0 || price > MAX_PRICE))
    {
        fprintf(stderr, "Price is not divisble by %i cents OR "
                "price is <= 0 or > %i cents!\n",CENT_ROUNDING, MAX_PRICE);
        free(stock);
        return NULL;
    }
    stock->price.dollars = price / DOLLAR_TO_CENT;
    stock->price.cents = (price % DOLLAR_TO_CENT);

    if((on_hand = get_on_hand(attributes[ONHAND])) > MAX_STOCK_LEVEL)
    {
        fprintf(stderr, "Stock level is > %i!\n",MAX_STOCK_LEVEL);
        free(stock);
        return NULL;
    }
    stock->on_hand = on_hand;

    return stock;
}

BOOLEAN check_stock_id_in_system(struct ppd_list *list, struct ppd_stock *data)
{
    struct ppd_node * current;
    current = list->head;

    while(current != NULL)
    {
        if(strcmp(current->data->id, data->id) == 0)
        {
            return TRUE;
        }
        current = current->next;
    }
    return FALSE;
}

BOOLEAN check_stock_id_regex(char * string)
{
    int i;
    if(string[0] != ID_CHAR)
        return FALSE;
    for(i = 1; i < IDLEN; ++i)
        if(string[i] < '0' || string[i] > '9')
            return FALSE;
    return TRUE;
}

BOOLEAN get_next_stock_id(struct ppd_list * list, char * next_id)
{
    char current_id[IDLEN +1] = FIRST_ID, * end;
    struct ppd_node * current = list->head;
    int id_num;

    while(current != NULL)
    {
        if(strcmp(current->data->id, current_id) > 0)
        {
            strcpy(current_id, current->data->id);
        }
        current = current->next;
    }
    if(list->head == NULL) {
        strcpy(next_id, current_id);
        return TRUE;
    }
    if(strcmp(current_id, LAST_ID) == 0)
    {
        return FALSE;
    }

    id_num = ((int) strtol(current_id + 1, &end, 0)) + 1;
    assert(*end == '\0');
    sprintf(next_id, "%c%04i", ID_CHAR, id_num);

    return TRUE;
}

BOOLEAN set_stock_level(struct ppd_list * list, struct ppd_stock * to_set,
                     unsigned amount)
{
    struct ppd_node * current = list->head;

    while(current != NULL)
    {
        if(current->data == to_set)
        {
            current->data->on_hand = amount;
            return TRUE;
        }
        current = current->next;
    }
    return FALSE;

}

void reset_all_stock_level(struct ppd_list * list)
{
    struct ppd_node * current = list->head;

    while(current != NULL)
    {
        current->data->on_hand = DEFAULT_STOCK_LEVEL;
        current = current->next;
    }
}

BOOLEAN check_price(char * price)
{
    int i = 0, cent_count = 0;
    BOOLEAN reached_delim = FALSE;

    if (count_delim_in_string(price, PRICE_DELIM_CHAR) != NO_PRICE_DELIM)
    {
        fprintf(stderr, "Invalid number of delimiter (%c) in price field!\n",
                PRICE_DELIM_CHAR);
        return FALSE;
    }

    while(price[i])
    {
        if(reached_delim)
            ++ cent_count;

        if(!isdigit(price[i]) && (price[i] != PRICE_DELIM_CHAR))
        {
            fprintf(stderr, "Invalid character in price field!\n");
            return FALSE;
        }

        if(price[i] == PRICE_DELIM_CHAR)
            reached_delim = TRUE;

        ++i;
    }

    if(cent_count != NO_CENTS)
    {
        fprintf(stderr, "Invalid format in price field!"
                " Must have 2 digit in cent field\n");
        return FALSE;
    }
    return TRUE;
}

BOOLEAN check_on_hand(char * onhand)
{
    int i = 0;

    while(onhand[i])
    {
        if(!isdigit(onhand[i]))
        {
            fprintf(stderr, "Invalid character/s in 'on hand' field!\n");
            return FALSE;
        }
        ++ i;
    }

    return TRUE;
}

unsigned get_price(const char * str)
{
    char * end;
    unsigned dollar, cent;

    dollar = (unsigned) strtol(str, &end, 0);
    cent = (unsigned) strtol(end + 1, &end, 0);

    return dollar * DOLLAR_TO_CENT + cent;
}

unsigned get_on_hand(const char * str)
{
    char * end;
    unsigned onhand;

    onhand = (unsigned) strtol(str, &end, 0);

    return onhand;
}
