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

#include "ppd_menu.h"
#include "ppd_options.h"
/**
 * @file ppd_menu.c handles the initialised and management of the menu 
 * array
 **/
BOOLEAN (*option_functions[NUM_OPTIONS])(struct ppd_system *) =
{
    display_items, purchase_item, save_system, add_item,
    remove_item, display_coins, reset_stock, reset_coins, NULL
};


const char * options_display[NUM_OPTIONS] =
{
    "1. Display Items",
    "2. Purchase Items",
    "3. Save and Exit",
    "4. Add Item",
    "5. Remove Item",
    "6. Display Coins",
    "7. Reset Stock",
    "8. Reset Coins",
    "9. Abort Program"
};

/**
 * @param menu the menu item array to initialise
 **/
void init_menu( struct menu_item* menu)
{
    int i;
    for(i = 0; i < NUM_OPTIONS; ++i)
    {
        strcpy((menu + i)->name, options_display[i]);
        (menu + i)->function = option_functions[i];
    }
}

void display_menu(struct menu_item * menu)
{
    int i;
    printf("\nMain Menu\n");
    for(i = 0; i < NUM_OPTIONS; ++i)
    {
        if(i == ADMIN_INDEX)
            printf("Administrator-Only Menu:\n");
        printf("    %s\n", (menu + i)->name);
    }
}
