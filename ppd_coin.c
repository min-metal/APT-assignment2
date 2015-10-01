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

/* array of valid denomination in cents */
const int VALID_DENOM[NUM_DENOMS] =
        {5, 10, 20, 50, 100, 200, 500, 1000};

const char *DENOM_STRING[] = {
        "5 cents", "10 cents", "20 cents", "50 cents",
        "1 dollar", "2 dollar", "5 dollar", "10 dollar"
};

void init_register(struct coin cash_register[])
{
    int i;
    for (i = 0; i < NUM_DENOMS; ++i)
    {
        cash_register[i].denom = (enum denomination) i;
        cash_register[i].count = 0;
    }
}

BOOLEAN print_register(struct coin cashregister[], BOOLEAN omit_empty)
{
    int i;
    printf("\nCoins Summary\n");
    printf("----------------------\n");
    printf("Denomination   | Count\n");
    printf("----------------------\n");

    for(i = 0; i < NUM_DENOMS; ++i)
    {
        if(cashregister[i].count == 0 && omit_empty)
            continue;

        printf("%-15s| %i\n",
               DENOM_STRING[cashregister[i].denom], cashregister[i].count);
    }
    return TRUE;
}

BOOLEAN add_to_register_from_string(struct coin *cash_register,
                        char attributes[][COIN_LENGTH + 1], BOOLEAN is_added[])
{
    char *current, * end;
    long denom, qty;
    int i = 0;

    current = attributes[DENOMINATION];
    while(*current)
    {
        if(!isdigit(*current))
        {
            fprintf(stderr, "One or more digit is not numeric in "
                    "denomination field!\n");
            return FALSE;
        }
        ++ current;
    }
    current = attributes[QTY];
    while(*current)
    {
        if(!isdigit(*current))
        {
            fprintf(stderr, "One or more digit is not numeric in "
                    "quantity field!\n");
            return FALSE;
        }
        ++ current;
    }

    denom = strtol(attributes[DENOMINATION], &end, 0);
    qty = strtol(attributes[QTY], &end, 0);
    if(qty < 0)
    {
        fprintf(stderr, "Quantity of coin is < 0!\n");
        return FALSE;
    }

    while(i < NUM_DENOMS)
    {
        if((int) denom == VALID_DENOM[i])
            break;
        ++i;
    }
    if(i == NUM_DENOMS)
    {
        fprintf(stderr, "Denomination is invalid!\n");
        return FALSE;
    }

    cash_register[i].count = (unsigned) qty;
    is_added[i] = TRUE;

    return TRUE;
}

BOOLEAN add_to_register(struct coin cash_register[], struct coin to_add[])
{
    int i;

    /* doing the checks before altering cash_register*/
    for(i = 0; i < NUM_DENOMS; ++i)
    {
        if(cash_register[i].denom != to_add[i].denom ||
           cash_register[i].count + to_add[i].count > MAX_COIN_QTY)
            return FALSE;
    }

    for(i = 0; i < NUM_DENOMS; ++i)
    {
        cash_register[i].count += to_add[i].count;
    }
    return TRUE;
}

BOOLEAN remove_from_register(struct coin cash_register[], struct coin to_remove[])
{
    int i;

    /* doing the checks before altering cash_register*/
    for(i = 0; i < NUM_DENOMS; ++i)
    {
        if(cash_register[i].denom != to_remove[i].denom ||
           (long)cash_register[i].count - (long)to_remove[i].count < 0)
            return FALSE;
    }

    for(i = 0; i < NUM_DENOMS; ++i)
    {
        cash_register[i].count -= to_remove[i].count;
    }
    return TRUE;
}

BOOLEAN get_change(int change_needed, struct coin cash_register[], struct coin change[])
{
    int i, change_remaining = change_needed;
    struct coin temp[NUM_DENOMS], cash_register_copy[NUM_DENOMS];

    init_register(temp);
    init_register(cash_register_copy);
    for(i = 0; i < NUM_DENOMS; ++i)
        cash_register_copy[i].count = cash_register[i].count;

    for(i = NUM_DENOMS - 1; i >= 0; --i)
    {
        while (change_remaining >= VALID_DENOM[i])
        {
            if(cash_register_copy[i].count == 0)
                break;

            change_remaining -= VALID_DENOM[i];
            ++temp[i].count;
            --cash_register_copy[i].count;
        }
    }

    if(change_remaining != 0)
        return FALSE;

    for(i = 0; i < NUM_DENOMS; ++i)
        change[i].count = temp[i].count;

    for(i = 0; i < NUM_DENOMS; ++i)
        cash_register[i].count = cash_register_copy[i].count;

    return TRUE;
}

BOOLEAN load_coin(struct coin cash_register[NUM_DENOMS], char * string)
{
    int number_of_lines = 0, i, successful_read = 0;
    size_t str_len;
    char * start = NULL;
    char temp[NO_COIN_ATTRIBUTE][COIN_LENGTH + 1];
    /* array of boolean to check which denomination has been added*/
    BOOLEAN is_added[NUM_DENOMS];
    memset(is_added, 0, sizeof(is_added));

    number_of_lines = explode_input(string, '\n') - 1;
    printf("No. lines to read from coins: %i\n", number_of_lines);

    start = string;
    while(*start)
    {
        if(count_delim_in_string(start, COIN_DELIM) != NO_COIN_DELIM)
        {
            fprintf(stderr, "Wrong number of delimiters! Must have %i '%c'.\n"
                    , NO_COIN_DELIM, COIN_DELIM);
            printf("Each data entry must end in '\\n', with no empty lines.\n");
            return FALSE;
        }

        explode_input(start, COIN_DELIM);
        for(i = 0; i < NO_COIN_ATTRIBUTE; ++i)
        {
            if ((str_len = strlen(start)) > COIN_LENGTH)
            {
                fprintf(stderr, "One or more field contains too many characters\n");
                return FALSE;
            }
            strcpy(temp[i], start);
            start += str_len + 1;
        }

        if(add_to_register_from_string(cash_register, temp, is_added) == FALSE)
        {
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

    for(i = 0; i < NUM_DENOMS; ++i)
    {
        if(is_added[i] == FALSE)
        {
            fprintf(stderr, "Error in coin file, "
                    "some coins denomination have not been added!\n");
            return FALSE;
        }
    }

    printf("Load successful\n");
    return TRUE;
}

BOOLEAN is_valid_denom(int denom, enum denomination * denom_enum)
{
    int i = 0;

    while(i < NUM_DENOMS)
    {
        if(denom == VALID_DENOM[i])
        {
            *denom_enum = (enum denomination) i;
            break;
        }
        ++i;
    }
    if(i == NUM_DENOMS)
    {
        return FALSE;
    }

    return TRUE;
}
