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

/**
 * @file ppd_coin.h defines the coin structure for managing currency in the
 * system. You should declare function prototypes for managing coins here
 * and implement them in ppd_coin.c
 **/
#ifndef PPD_COIN
#define PPD_COIN
#include "ppd_main.h"
#define COIN_DELIM ','
#define COIN_DELIM_STRING ","
/* number of coin delims per line*/
#define NO_COIN_DELIM 1
#define NO_COIN_ATTRIBUTE 2

/* max chars for coin except nul*/
#define COIN_LENGTH 5

/* max number of coin count */
#define MAX_COIN_QTY 9999
/**
 * The number of denominations of currency available in the system
 **/
#define NUM_DENOMS 8

struct ppd_system;
/**
 * enumeration representing the various types of currency available in
 * the system. 
 **/

typedef enum coin_attribute
{
    DENOMINATION, QTY
} COIN_ATTRIBUTE;

enum denomination
{
    FIVE_CENTS, TEN_CENTS, TWENTY_CENTS, FIFTY_CENTS, ONE_DOLLAR, 
    TWO_DOLLARS, FIVE_DOLLARS, TEN_DOLLARS
};

/**
 * represents a coin type stored in the cash register. Each denomination
 * will have exactly one of these in the cash register.
 **/
struct coin
{
    /**
     * the denomination type
     **/
    enum denomination denom;
    /**
     * the count of how many of these are in the cash register
     **/
    unsigned count;
};

void init_register(struct coin cash_register[]);
BOOLEAN print_register(struct coin cash_register[], BOOLEAN);
BOOLEAN is_valid_denom(int denom, enum denomination * denom_enum);
BOOLEAN load_coin(struct coin cash_register[NUM_DENOMS], char * string);
BOOLEAN serialize_coin(const char * file_name, struct coin cash_register[]);

BOOLEAN add_to_register_from_string(struct coin *cash_register,
    char attributes[][COIN_LENGTH + 1], BOOLEAN *is_added);

BOOLEAN add_to_register(struct coin cash_register[], struct coin to_add[]);
BOOLEAN remove_from_register(struct coin cash_register[],
    struct coin to_remove[]);
void reset_register(struct coin cash_register[]);

BOOLEAN get_change(int change_needed, struct coin cash_register[],
    struct coin change[]);
#endif
