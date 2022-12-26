#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 100

typedef char string[MAX]; // defines a new data type "string" as a character array with "MAX" number of elements

typedef struct Date // struct that stores the date as an integer month and integer year
{
    int month, year;

} Date;

typedef struct Book
{
    char ISBN[14]; // character array ISBN to store the 13 digit ISBN number as a string with the 14th element being "\0"
    string name;
    string author;
    int quantity;
    float price;
    Date publication; // struct publication date that includes month and year
} Book;

typedef struct Account // struct that holds account information (username and password)
{
    string username;
    string password;
} Account;

Book info[MAX]; // global array of struct Book with up to MAX number of elements
Account Credentials[MAX]; // global array of struct Account with up to MAX number of elements

int numOfLines(FILE *fileCredentials) // function that counts the number of lines in a file
{
    rewind(fileCredentials); // function repositions the file pointer to the beginning of the file
    int count = 1;
    char ch;

    while( (ch = fgetc(fileCredentials)) != EOF) // while loop to count number of lines
        if( ch == '\n')
            count++;
    return count;
}

void readCredentials(FILE *fileCredentials, int n) // function that stores user name and password information into the Credentials struct
{
    rewind(fileCredentials); // function repositions the file pointer to the beginning of the file
    int i,j;
    for(i =0 ; !feof(fileCredentials) ; i++) // for loop that moves through to load credentials array with username and password data from the credentials file
    {
        fgets(Credentials[i].username, sizeof(Credentials[i].username), fileCredentials); //copies line from file into the username string of struct Account Credentials[i]
        Credentials[i].username[strlen(Credentials[i].username)-1] = '\0'; // removes \n that fgets puts at the end of the string
        fgets(Credentials[i].password, sizeof(Credentials[i].password), fileCredentials); //copies line from file into the password string of struct Account Credentials[i]
        Credentials[i].password[strlen(Credentials[i].password)-1] = '\0'; // removes \n that fgets puts at the end of the string
    }

}

int login(int trials, int n) // login function that prompts user to enter a username and password and returns 1 if the user inputs valid credentials
{
    int i;
    if (trials == 0) // condition that checks whether the user used up the limited number of trials
    {
        printf("Try again later.");
        return 0;
    }
    string user, pass;
    printf("Enter Username: ");
    gets(user);
    printf("Enter Password: ");
    gets(pass);
    for(i =0; i < n; i++) // for loop that passes through the Credentials array
        if (!strcmp(Credentials[i].username, user) && !strcmp(Credentials[i].password, pass)) // BASE CASE - condition to check if the user inputted username and password match with any username and password from credentials array
            return 1; // leaves function by returning 1 and skips the remaining lines of the function without executing
    printf("Invalid username or password\nYou have %d more trials\n\n\n", trials-1); //prints error message informing the user of how many tries they have left before the program closes
    login(--trials, n); // RECURSIVE CASE - in case of incorrect credentials, reduces number of tries remaining by 1 before function calls itself again

}

void load(FILE * fileBooks, int n) // load function that stores book.txt file data into the global array "info"
{
    rewind(fileBooks); // function repositions the file pointer to the beginning of the file
    string temp;
    int i;
    for (i=0; i< n; i++)
    {
        fgets(temp, MAX, fileBooks);

        strcpy(info[i].ISBN, strtok(temp, ","));
        strcpy(info[i].name, strtok(NULL, ","));
        strcpy(info[i].author, strtok(NULL, ","));

        info[i].quantity = atoi(strtok(NULL, ","));
        info[i].price = atof(strtok(NULL, ","));
        info[i].publication.month = atoi(strtok(NULL, "-"));
        info[i].publication.year = atoi(strtok(NULL, "-"));
    }
}

int main()
{
    int i, nLines;

    FILE * fileCredentials = fopen("files//credentials.txt", "r"); // initializes file pointer that opens credentials.txt file to be read only
    FILE * fileBooks = fopen("files//books.txt", "r+"); // initializes file pointer that opens books.txt file
    if (fileCredentials == NULL)
    {
        printf("Error while opening the file"); // Error message if credentials.txt file is not found
        return 1; // Ends program by returning 1 to show error
    }
    if (fileBooks == NULL)
    {
        printf("Error while opening the books.txt file"); // Error message if books.txt file is not found
        return 1; // Ends program by returning 1 to show error
    }
    nLines = numOfLines(fileCredentials); // declares and intializes integer to number of lines in credentials file

    readCredentials(fileCredentials, nLines / 2); // calls function to store username and password data into the global credentials array - gives function half number of lines since each element of the credentials array stores 2 lines
    fclose(fileCredentials); // closes credentials.txt file

    if(login(5, nLines / 2))
        printf("DONE\n");
    else
        return 1;

    nLines = numOfLines(fileBooks); // integer that gets number of lines in books.txt file
    load(fileBooks, nLines); // calls function to store books.txt data into the global array "info"

    return 0;
}


