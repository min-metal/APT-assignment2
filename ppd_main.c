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

#include "ppd_main.h"
#include "ppd_menu.h"
#include "ppd_options.h"
#include "ppd_utility.h"
#include "ppd_coin.h"
#include "ppd_stock.h"

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
    FILE * stock, * coins;

    /* validate command line arguments */
    if (argc != /*3*/ 1)
    {
        printf("Invalid arguments\n");
        return EXIT_FAILURE;
    }

    /* Remember to fix */
    if ((stock = fopen(/*argv[1]*/ PATH "stock.dat", "r")) == NULL)
    {
        fprintf(stderr, "File open error\n");
        return EXIT_FAILURE;
    }
    if ((coins = fopen(/*argv[2]*/ PATH "coins.dat", "r")) == NULL)
    {
        fprintf(stderr, "File open error\n");
        return EXIT_FAILURE;
    }

/*    while((c = getc(coins)) != EOF)
    {
        printf("%c", c);
    }*/


    /* init the system */
    if(system_init(&system) == FALSE)
    {
        return EXIT_FAILURE;
    }


    /* load data */
    if (load_data(&system, "coins.dat", "stock.dat") == FALSE)
    {
        printf("Load data failed!\n");
        system_free(&system);
        fclose(stock);
        fclose(coins);
        return EXIT_FAILURE;
    }

    display_list(system.item_list);


    /* test if everything has been initialised correctly */

    /* initialise the menu system */

    /* loop, asking for options from the menu */

    /* run each option selected */

    /* until the user quits */

    /* make sure you always free all memory and close all files 
     * before you exit the program
     */


    system_free(&system);
    fclose(stock);
    fclose(coins);
    return EXIT_SUCCESS;
}
