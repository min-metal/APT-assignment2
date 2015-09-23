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
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <ctype.h>
#include "ppd_coin.h"
#include "ppd_main.h"


#ifndef PPD_STOCK
#define PPD_STOCK

/**
 * @file ppd_stock.h this file defines the data structures required to 
 * manage the stock list. You should add here the function prototypes for
 * managing the list here and implement them in ppd_stock.c
 **/

#define STOCK_DELIM '|'
#define STOCK_DELIM_STRING "|"

/* number of inputs per line, number of attributes */
#define NO_ATTRIBUTE 5
/* number of delims */
#define NO_STOCK_DELIMS NO_ATTRIBUTE - 1

/**
 * The length of the id string not counting the nul terminator
 **/
#define IDLEN 5

/**
 * The maximum length of a product name not counting the nul terminator
 **/
#define NAMELEN 40

/**
 * The maximum length of a product description not counting the nul
 * terminator.
 **/
#define DESCLEN 255

#define PRICELEN 5
/* max price in cents */
#define MAX_PRICE 9999
#define PRICE_DELIM_CHAR '.'
#define NO_PRICE_DELIM 1

/* how many characters a cent field should have */
#define NO_CENTS 2
/* price should divisible by 5 cents. */
#define CENT_ROUNDING 5
/* dollar * 100 = cents */
#define DOLLAR_TO_CENT 100

#define ONHANDLEN 5

#define EXTRA_SPACE 1
/**
 * The default coin level to reset the coins to on request
 **/
#define DEFAULT_COIN_COUNT 20

/**
 * The default stock level that all new stock should start at and that 
 * we should reset to on restock
 **/
#define DEFAULT_STOCK_LEVEL 20
#define MAX_STOCK_LEVEL 99

#define STOCK_LINE_LEN IDLEN + NAMELEN + DESCLEN + 20

/**
 * The number of denominations of currency available in the system 
 **/
#define NUM_DENOMS 8

typedef enum attribute
{
    ID, NAME, DESC, PRICE, ONHAND
} ATTRIBUTE;


/**
 * a structure to represent a price. One of the problems with the floating
 * point formats in C like float and double is that they have minor issues
 * of inaccuracy due to rounding. In the case of currency this really is
 * not acceptable so we introduce our own type to keep track of currency.
 **/
struct price
{
    /**
     * the dollar value for some price
     **/
    unsigned dollars, 
             /**
              * the cents value for some price
              **/
             cents;
};

/**
 * data structure to represent a stock item within the system
 **/
struct ppd_stock
{
    /**
     * the unique id for this item
     **/
    char id[IDLEN + 1];
    /**
     * the name of this item
     **/
    char name[NAMELEN + 1];
    /**
     * the description of this item
     **/
    char desc[DESCLEN + 1];
    /**
     * the price of this item
     **/
    struct price price;
    /**
     * how many of this item do we have on hand? 
     **/
    unsigned on_hand;
    /**
     * a pointer to the next node in the list
     **/
};

/**
 * the node that holds the data about an item stored in memory
 **/
struct ppd_node
{
    struct ppd_stock * data;
    struct ppd_node * next;
};

/**
 * the list of products - each link is the list is a @ref ppd_node
 **/
struct ppd_list
{
    /**
     * the beginning of the list
     **/
    struct ppd_node * head;
    /**
     * how many nodes are there in the list?
     **/
    unsigned count;
};

/**
 * this is the header structure for all the datatypes that is
 * passed around and manipulated
 **/
struct ppd_system
{
    /**
     * the container for all the money manipulated by the system
     **/
    struct coin cash_register[NUM_DENOMS];

    /**
     * the linked list - note that this is a pointer - how does that
     * change what we need to do for initialization of the list?
     **/
    struct ppd_list * item_list;

    /**
     * the name of the coin file - we need this for saving as all menu
     * items only take the one parameter of the ppd_system
     **/
    const char * coin_file_name;
    /**
     * the name of the stock file
     **/
    const char * stock_file_name;
};

BOOLEAN add_to_list(struct ppd_list * list, struct ppd_stock * data);
BOOLEAN display_list(struct ppd_list * list);

/* integrity check before adding to list */
BOOLEAN load_stock(struct ppd_list * list, char * string);

struct ppd_stock * new_stock(char attributes[][DESCLEN + EXTRA_SPACE]);

BOOLEAN check_stock_id(struct ppd_list * list, struct ppd_stock * data);
BOOLEAN check_price(char * price);
BOOLEAN check_on_hand(char * onhand);

/* returns price in cents, presumes input is valid,
 * does not check if cent%5 == 0;
 */
unsigned get_price(const char *str);
unsigned get_on_hand(const char * str);
#endif



