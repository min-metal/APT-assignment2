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
    char * string;
    system->coin_file_name = coins_name;
    system->stock_file_name = stock_name;

    string = read_from_file(stock_name);
    if(string)
    {
        /*puts(string);*/
        if (load_stock(system->item_list, string) == FALSE)
        {
            free(string);
            return FALSE;
        }
        free(string);
    }

    string = read_from_file(coins_name);
    if(string)
    {
        /*puts(string);*/
        if(load_coin(system->cash_register, string) == FALSE)
        {
            free(string);
            return FALSE;
        }
        free(string);
    }

    return TRUE;
}

/**
 * @param system a pointer to a @ref ppd_system struct that holds all 
 * the data for the system we are creating
 **/
BOOLEAN system_init(struct ppd_system * system)
{
    init_register(system->cash_register);
    system->item_list = safe_malloc(sizeof(struct ppd_list));
    system->item_list->head = NULL;
    system->item_list->count = 0;
    system->stock_file_name = NULL;
    system->coin_file_name = NULL;
    return TRUE;
}

/**
 * @param system a pointer to a @ref ppd_system struct that holds all 
 * the data for the system we are creating
 **/
void system_free(struct ppd_system * system)
{
    struct ppd_node * current, * next;

    current = system->item_list->head;
    while(current != NULL)
    {
        next = current->next;
        free(current->data);
        free(current);
        current = next;
    }

    free(system->item_list);

}

void * safe_malloc(size_t size)
{
    void * returnPointer;
    if((returnPointer = malloc(size)) == NULL)
    {
        printf("Malloc failed\n");
        exit(EXIT_FAILURE);
    }
    else
    {
        return returnPointer;
    }
}

/* sourced from http://stackoverflow.com/questions/3463426/in-c-how-should-i-read-a-text-file-and-print-all-strings */
/* from user 'lfzawacki', answered Aug 12 '10 at 3:56, accessed 19/09/15, used for educational purposes only */
char * read_from_file(const char * file_name)
{
    /* TODO fix file path */
    FILE * ifp;
    char file_path[100] = PATH;
    char * buffer = NULL;
    long string_size;

    strcat(file_path, file_name);

    ifp = fopen(/*file_name*/ file_path, "r");

    fseek(ifp, 0, SEEK_END);
    string_size = ftell(ifp);
    rewind(ifp);

    buffer = (char *) safe_malloc(sizeof(char) * string_size + 1);
    memset(buffer, 0, sizeof(char) * string_size + 1);

    if(fread(buffer, sizeof(char), (size_t) string_size, ifp) != string_size)
    {
        free(buffer);
        buffer = NULL;
    }

    fclose(ifp);
    return buffer;
}

INPUT_RESULT getString(char* string, unsigned length, char* prompt)
{
    int finished = FALSE;
    char* tempString;

    /* Allocate temporary memory. */
    if ((tempString = malloc(sizeof(char) * (length+2))) == NULL)
    {
        fprintf(stderr, "Fatal error: malloc() failed in getString().\n");
        exit(EXIT_FAILURE);
    }

    /* Continue to interact with the user until the input is valid. */
    do
    {
        /* Provide a custom prompt. */
        printf("%s", prompt);

        /* Accept input. "+2" is for the \n and \0 characters. */
        if(fgets(tempString, length + 2, stdin) == NULL)
        {
            free(tempString);
            return RTM;
        }

        /* A string that doesn't have a newline character is too long. */
        if (tempString[strlen(tempString) - 1] != '\n')
        {
            printf("Input was too long.\n");
            read_rest_of_line();
        }
        else if (tempString[0] == '\n')
        {
            free(tempString);
            return RTM;
        }
        else
        {
            finished = TRUE;
        }

    } while (finished == FALSE);

    /* Overwrite the \n character with \0. */
    tempString[strlen(tempString) - 1] = '\0';

    /* Make the result string available to calling function. */
    strcpy(string, tempString);

    /* Deallocate temporary memory. */
    free(tempString);

    return SUCCESS;
}

INPUT_RESULT get_int(int * num, unsigned length, char * prompt)
{
    char * temp, * end;
    INPUT_RESULT input_result;
    int input_num;

    temp = safe_malloc(sizeof(char) * (length + 2));

    if((input_result = getString(temp, length, prompt)) != SUCCESS)
    {
        free(temp);
        return input_result;
    }

    input_num = (int) strtol(temp, &end, 0);

    if(*end)
    {
        printf("Data is not numeric!\n");
        free(temp);
        return FAILURE;
    }

    *num = input_num;
    free(temp);
    return SUCCESS;
}


int explode_input(char * string, const char delim)
{
    int count = 1;
    char * token;
    token = string;
/*    token = strtok(string, delim);

    while(token != NULL)
    {
        printf( "%s\n", token ); TODO remove printf
        token = strtok(NULL, delim);
        ++count;
    }*/

    while(*token != '\0')
    {
        if(*token == delim)
        {
            *token = '\0';
            ++count;
        }
        ++token;
    }
    return count;
}

int count_delim_in_string(const void * str, int c)
{
    int count = 0;
    const char * p = str;

    while((p = strchr(p, c)) != NULL)
    {
        ++count;
        ++p; /* increment pointer to char after delim */
    }

    return count;
}
