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

#include "ppd_menu.h"
#include "ppd_options.h"

/**
 * @file ppd_main.c contains the main function implementation and any 
 * helper functions for main such as a display_usage() function.
 **/

/**
 * manages the running of the program, initialises data structures, loads
 * data and handles the processing of options. The bulk of this function
 * should simply be calling other functions to get the job done.
 **/

int main(int argc, char ** argv)
{
    /* represents the data structures to manage the system */
    struct ppd_system system;
    struct menu_item menu[NUM_OPTIONS];
    FILE * stock, * coins;

    /* validate command line arguments */
    if (argc != 3)
    {
        printf("Invalid arguments\n");
        printf("Expecting ./ppd <itemsfile> <coinsfile>\n");
        return EXIT_FAILURE;
    }

    /* TODO Remember to fix */
    if ((stock = fopen(argv[1] /*PATH "stock.dat"*/, "r")) == NULL)
    {
        fprintf(stderr, "File open error\n");
        return EXIT_FAILURE;
    }
    if ((coins = fopen(argv[2] /*PATH "coins.dat"*/, "r")) == NULL)
    {
        fprintf(stderr, "File open error\n");
        return EXIT_FAILURE;
    }

    /* init the system */
    if(system_init(&system) == FALSE)
    {
        return EXIT_FAILURE;
    }


    /* test if everything has been initialised correctly */
    if (load_data(&system, argv[2]/*"coins.dat"*/, argv[1]/*"stock.dat"*/) == FALSE)
    {
        printf("Load data failed!\n");
        system_free(&system);
        fclose(stock);
        fclose(coins);
        return EXIT_FAILURE;
    }

    /* initialise the menu system */
    init_menu(menu);

    /* loop, asking for options from the menu */
    while(1)
    {
        int chosen;
        INPUT_RESULT input_result;
        BOOLEAN operation_result;

        display_menu(menu);
        input_result=get_int(&chosen,DEFAULT_INPUT_LEN,"Select your option: ");
        if(input_result != SUCCESS ||
                chosen < 0 || chosen > NUM_OPTIONS)
            continue;

        if(menu[chosen - 1].function == NULL)
            break;

        /* run each option selected */
        operation_result = menu[chosen - 1].function(&system);
        if(operation_result)
            printf("Operation successful.\n");
        else
            printf("Operation failed.\n");

        /* until the user quits */
        if(menu[chosen - 1].function == save_system)
            break;
    }

    /* make sure you always free all memory and close all files 
     * before you exit the program
     */
    system_free(&system);
    fclose(stock);
    fclose(coins);
    return EXIT_SUCCESS;
}
