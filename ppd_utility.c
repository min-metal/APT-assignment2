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

#include "ppd_utility.h"
/**
 * @file ppd_utility.c contains implementations of important functions for
 * the system. If you are not sure where to implement a new function, 
 * here is probably a good spot.
 **/

void read_rest_of_line(void)
{
    int ch;
    /* keep retrieving characters from stdin until we
     * are at the end of the buffer
     */
    while(ch = getc(stdin), ch!='\n' && ch != EOF)
        ;
    /* reset error flags on stdin */
    clearerr(stdin);
}

/**
 * @param system a pointer to a @ref ppd_system struct that contains
 * all the data in the system we are manipulating
 * @param coins_name the name of the coins file
 * @param stock name the name of the stock file
 **/
BOOLEAN load_data(struct ppd_system * system , const char * coins_name, 
const char * stock_name)
{
    /*
     * Please delete this default return value once this function has 
     * been implemented. Please note that it is convention that until
     * a function has been implemented it should return FALSE
     */
    return FALSE;
}

/**
 * @param system a pointer to a @ref ppd_system struct that holds all 
 * the data for the system we are creating
 **/
BOOLEAN system_init(struct ppd_system * system)
{
    /*
     * Please delete this default return value once this function has 
     * been implemented. Please note that it is convention that until
     * a function has been implemented it should return FALSE
     */
    return FALSE;
}

/**
 * @param system a pointer to a @ref ppd_system struct that holds all 
 * the data for the system we are creating
 **/
void system_free(struct ppd_system * system)
{

}
