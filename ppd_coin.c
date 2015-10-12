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
#include "ppd_utility.h"

/* array of valid denomination in cents */
const int VALID_DENOM[NUM_DENOMS] =
        {5, 10, 20, 50, 100, 200, 500, 1000};

const char *DENOM_STRING[] = {
        "5 cents", "10 cents", "20 cents", "50 cents",
        "1 dollar", "2 dollar", "5 dollar", "10 dollar"
};

/*
 * initializes the register, sets all count to 0
 */
void init_register(struct coin cash_register[])
{
    int i;
    for (i = 0; i < NUM_DENOMS; ++i)
    {
        cash_register[i].denom = (enum denomination) i;
        cash_register[i].count = 0;
    }
}

/*
 * formatted printing of register, option to omit denominations that are
 * empty
 */
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

/*
 * formatted printing of change given
 */
void print_change(struct coin change[])
{
    int i, total =0;
    for(i = 0; i < NUM_DENOMS; ++i)
    {
        total += VALID_DENOM[change[i].denom] * change[i].count;
    }

    printf("\nHere is your change of $%.2f: ", (float)total / DOLLAR_TO_CENT);
    for(i = NUM_DENOMS; i >= 0; --i)
    {
        if(change[i].count == 0)
            continue;

        printf("%s * %i",
               DENOM_STRING[change[i].denom], change[i].count);
        if(i != 0)
            printf(", ");
    }
    printf(".\n");
}

/*
 * convert array of strings, which contains denom and qty, and add it to
 * cash_register, set is_added[i] to TRUE, where i = (enum denom) of
 * coin added.
 */
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

/*
 * adding coins to cash register, from another array of struct coin,
 * the denomination of both should be the same.
 */
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

/*
 * removing coins from cash_register from another struct coin array,
 * the denomination in both should be the same. returns FALSE, if
 * removing coin will result in value < 0
 */
BOOLEAN remove_from_register(struct coin cash_register[],
                             struct coin to_remove[])
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

/*
 * reset register to default coin count
 */
void reset_register(struct coin cash_register[])
{
    int i;

    for(i = 0; i < NUM_DENOMS; ++i)
    {
        cash_register[i].count = DEFAULT_COIN_COUNT;
    }
}

/*
 * gives change, sets struct coin change[] to amount of change needed,
 * removes change from cashe_register
 *
 * @param change_needed, amount of change needed in cents
 * @param cash_register, system cash register
 * @param change, array of struct coin to be altered, which equates to
 * change_needed when added up
 */
BOOLEAN get_change(int change_needed, struct coin cash_register[],
                   struct coin change[])
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

/*
 * does the loading from data string into the system
 */
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
            printf("Each data entry must end in '\\n', "
                           "with no empty lines.\n");
            return FALSE;
        }

        explode_input(start, COIN_DELIM);
        for(i = 0; i < NO_COIN_ATTRIBUTE; ++i)
        {
            if ((str_len = strlen(start)) > COIN_LENGTH)
            {
                fprintf(stderr, "One or more field contains too many "
                        "characters\n");
                return FALSE;
            }
            strcpy(temp[i], start);
            start += str_len + 1;
        }

        if(add_to_register_from_string(cash_register, temp, is_added)==FALSE)
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

/*
 * writes system cash_register to file
 */
BOOLEAN serialize_coin(const char * file_name, struct coin cash_register[])
{
    int i;
    FILE * fp;
    char coin_buffer[COIN_LENGTH + 1];

    fp = fopen(file_name, "w");
    for(i = 0; i < NUM_DENOMS; ++i)
    {
        sprintf(coin_buffer, "%i", VALID_DENOM[cash_register[i].denom]);
        fwrite(coin_buffer, sizeof(char), strlen(coin_buffer), fp);
        fwrite(COIN_DELIM_STRING, sizeof(char), 1, fp);
        sprintf(coin_buffer, "%u", cash_register[i].count);
        fwrite(coin_buffer, sizeof(char), strlen(coin_buffer), fp);
        fwrite("\n", sizeof(char), 1, fp);
    }
    fclose(fp);
    return TRUE;
}

/*
 * checks if int demon in cents is a valid denomination,
 * if valid, sets denom_enum to the corresponding enum denomination
 */
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
