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

#include "ppd_options.h"

/**
 * @file ppd_options.c this is where you need to implement the main
 * options for your program. You may however have the actual work done
 * in functions defined elsewhere. 
 * @note if there is an error you should handle it within the function
 * and not simply return FALSE unless it is a fatal error for the 
 * task at hand. You want people to use your software, afterall, and
 * badly behaving software doesn't get used.
 **/

/**
 * @param system a pointer to a  ppd_system struct that contains 
 * all the information for managing the system.
 * @return true as this particular function should never fail.
 **/
BOOLEAN display_items(struct ppd_system * system)
{
    assert(display_list(system->item_list));
    return TRUE;
}

/**
 * @param system a pointer to a  ppd_system struct that contains 
 * all the information for managing the system.
 * @return true when a purchase succeeds and false when it does not
 **/
BOOLEAN purchase_item(struct ppd_system * system)
{
    struct coin denom_given[NUM_DENOMS], denom_change[NUM_DENOMS];
    char item_id[IDLEN + EXTRA_SPACE];
    int money_given = 0, still_needed, change_needed;
    INPUT_RESULT input_result;
    struct ppd_stock * temp_stock = NULL;

    init_register(denom_given);
    init_register(denom_change);

    printf("\nPurchase Item\n");
    printf("-------------\n");

    input_result = getString(item_id, NAMELEN + EXTRA_SPACE,
             "Please enter the ID of the item you wish to purchase: ");
    if(input_result != SUCCESS) {
        printf("Returning to Menu!\n");
        return FALSE;
    }

    if((temp_stock = get_stock_by_id(system->item_list, item_id)) == NULL)
    {
        printf("Stock ID is not in system!\n");
        return FALSE;
    }
    if(temp_stock->on_hand == 0){
        printf("The item selected has been exhausted.\n");
        return FALSE;
    }

    printf("You have selected \"%s - %s\". This will cost you $%.2f.\n",
    temp_stock->name, temp_stock->desc,
    (float)(temp_stock->price.dollars*DOLLAR_TO_CENT+temp_stock->price.cents)/
           DOLLAR_TO_CENT
    );

    printf("Please hand over the money - "
                   "type in the value of each note/coins in cents.\n");

    still_needed =
        (temp_stock->price.dollars * DOLLAR_TO_CENT + temp_stock->price.cents)
        - money_given;
    while(still_needed > 0)
    {
        char prompt[PROMPT_LEN];
        enum denomination denom_enum;
        int temp_given = 0;

        sprintf(prompt, "You still need to give us $%.2f: ",
               ((float) still_needed / DOLLAR_TO_CENT));
        input_result = get_int(&temp_given, COIN_LENGTH, prompt);

        if(input_result == RTM)
            return FALSE;
        else if(input_result == FAILURE)
            continue;

        if(!is_valid_denom(temp_given, &denom_enum))
        {
            printf("Incorrect denomination!\n");
            continue;
        }
        else
        {
            ++denom_given[denom_enum].count;
            still_needed -= temp_given;
        }
    }

    if(add_to_register(system->cash_register, denom_given) == FALSE)
    {
        printf("Adding coin to register failed, Register full!\n");
        return FALSE;
    }

    change_needed = still_needed * -1;

    if(!get_change(change_needed, system->cash_register, denom_change))
    {
        printf("System doesn't have enough change!\n");
        remove_from_register(system->cash_register, denom_given);
        return FALSE;
    }

    assert(set_stock_level(system->item_list, temp_stock,
        temp_stock->on_hand - 1));
    printf("\n/** CHANGE GIVEN **/");
    /*print_register(denom_change, TRUE);*/
    print_change(denom_change);
    return TRUE;
}

/**
 * @param system a pointer to a  ppd_system struct that contains 
 * all the information for managing the system.
 * @return true when a save succeeds and false when it does not
 **/
BOOLEAN save_system(struct ppd_system * system)
{
    FILE * fp;
    char price[PRICELEN + 1], onhand[ONHANDLEN + 1];
    struct ppd_node * current = system->item_list->head;

    fp = fopen(system->stock_file_name, "w");
    while(current != NULL)
    {
        fwrite(current->data->id, sizeof(char), strlen(current->data->id), fp);
        fwrite(STOCK_DELIM_STRING, sizeof(char), 1, fp);
        fwrite(current->data->name, sizeof(char), strlen(current->data->name), fp);
        fwrite(STOCK_DELIM_STRING, sizeof(char), 1, fp);
        fwrite(current->data->desc, sizeof(char), strlen(current->data->desc), fp);
        fwrite(STOCK_DELIM_STRING, sizeof(char), 1, fp);
        sprintf(price, "%u.%02u", current->data->price.dollars, current->data->price.cents);
        fwrite(price, sizeof(char), strlen(price), fp);
        fwrite(STOCK_DELIM_STRING, sizeof(char), 1, fp);
        sprintf(onhand, "%u", current->data->on_hand);
        fwrite(onhand, sizeof(char), strlen(onhand), fp);
        fwrite("\n", sizeof(char), 1, fp);
        current = current->next;
    }

    serialize_coin(system->coin_file_name, system->cash_register);

    fclose(fp);
    return TRUE;
}

/**
 * @param system a pointer to a  ppd_system struct that contains 
 * all the information for managing the system.
 * @return true when adding an item succeeds and false when it does not
 **/
BOOLEAN add_item(struct ppd_system * system)
{
    char buffer[NO_ATTRIBUTE][DESCLEN + EXTRA_SPACE];
    struct ppd_stock * new;

    if(get_next_stock_id(system->item_list, buffer[ID]) == FALSE)
    {
        printf("Cannot add anymore items.\n");
        return FALSE;
    }
    printf("This item will have an ID of (%s).\n", buffer[ID]);

    if(getString(buffer[NAME], NAMELEN, "Enter the item name: ") != SUCCESS) {
        return FALSE;
    }
    if(getString(buffer[DESC], DESCLEN,
        "Enter the item description: ") != SUCCESS) {
        return FALSE;
    }
    if(getString(buffer[PRICE], PRICELEN,
        "Enter the price for this item: ") != SUCCESS) {
        return FALSE;
    }
    sprintf(buffer[ONHAND], "%i", DEFAULT_STOCK_LEVEL);

    if(check_user_input(buffer) == FALSE)
    {
        fprintf(stderr, "Input must not contain '%c'.\n", STOCK_DELIM);
        return FALSE;
    }

    if((new = new_stock(buffer)) == NULL)
    {
        fprintf(stderr, "Stock creation unsuccessful.\n");
        return FALSE;
    }

    assert(add_to_list(system->item_list, new));

    printf("The item \"%s - %s\" has now been added to the menu.\n",
           new->name, new->desc);

    return TRUE;
}

/**
 * @param system a pointer to a  ppd_system struct that contains 
 * all the information for managing the system.
 * @return true when removing an item succeeds and false when it does not
 **/
BOOLEAN remove_item(struct ppd_system * system)
{
    struct ppd_stock * to_remove;
    char id[IDLEN + 1];

    if(getString(id, IDLEN, "Enter ID of the item you wish to remove: ")
       != SUCCESS) {
        return FALSE;
    }

    if((to_remove = get_stock_by_id(system->item_list, id)) == NULL)
    {
        printf("ID does not exist!\n");
        return FALSE;
    }

    printf("The item \"%s - %s\" has been removed from the system.\n",
           to_remove->name, to_remove->desc);

    assert(remove_from_list(system->item_list, to_remove));

    return TRUE;
}

/**
 * @param system a pointer to a  ppd_system struct that contains 
 * all the information for managing the system.
 * @return true as this function cannot fail.
 **/
BOOLEAN reset_stock(struct ppd_system * system)
{
    reset_all_stock_level(system->item_list);
    return TRUE;
}

/**
 * @param system a pointer to a  ppd_system struct that contains 
 * all the information for managing the system.
 * @return true as this function cannot fail.
 **/
BOOLEAN reset_coins(struct ppd_system * system)
{
    reset_register(system->cash_register);
    return TRUE;
}

/**
 * @param system a pointer to a  ppd_system struct that contains 
 * all the information for managing the system.
 * @return true as this function cannot fail
 **/
BOOLEAN display_coins(struct ppd_system * system)
{
    print_register(system->cash_register, FALSE);
    return TRUE;
}
