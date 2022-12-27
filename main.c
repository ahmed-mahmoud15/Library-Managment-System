#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 100
#define numofTrials 5

typedef char string[MAX]; // defines a new data type "string" as a character array with "MAX" number of elements

typedef struct Date // struct that stores the date as an integer month and integer year
{
    int month, year;

} Date;

typedef struct Book
{
    char ISBN[14]; // character array ISBN to store the 13 digit ISBN number as a string with the 14th element being "\0"
    string title;
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
int infoSize; // global variable for utilized size of info

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

void load(FILE * fileBooks) // load function that stores book.txt file data into the global array "info"
{
    rewind(fileBooks); // function repositions the file pointer to the beginning of the file
    string temp;
    int i;
    for (i=0; i< infoSize; i++)
    {
        fgets(temp, MAX, fileBooks);

        strcpy(info[i].ISBN, strtok(temp, ","));
        strcpy(info[i].title, strtok(NULL, ","));
        strcpy(info[i].author, strtok(NULL, ","));

        info[i].quantity = atoi(strtok(NULL, ","));
        info[i].price = atof(strtok(NULL, ","));
        info[i].publication.month = atoi(strtok(NULL, "-"));
        info[i].publication.year = atoi(strtok(NULL, "-"));
    }
}
void printDate(Date date)
{
    string months[12] =
    {
        "January", "February", "March", "April", "May", "June",
        "July", "August", "September", "October", "November", "December"
    };
    printf("Published: %s %d\n\n", months[date.month - 1], date.year);
}

void printBook (int i)
{
    printf("\nISBN: %s\n", info[i].ISBN);
    printf("Title: %s\n", info[i].title);
    printf("Author: %s\n", info[i].author);
    printf("Quantity: %d\n", info[i].quantity);
    printf("Price: %.2f$\n", info[i].price);
    printDate(info[i].publication);
}

void Query (string isbn)
{
    int i,flag=0;
    for(i=0; i<infoSize; i++)
    {
        if (strcmp(isbn, info[i].ISBN) == 0)
        {
            printBook(i);
            flag=1;
            break;
        }
    }
    if (flag==0)
    {
        printf("The ISBN entered is not available\n");
    }
}

void printAll()
{
    int i;
    printf("\nSort by:\nTitle\nPrice\nDate of Publication\n");
    printf("-----------------------------------------\n");
    for (i = 0; i < infoSize; i++)
    {
        printBook(i);
        printf("-----------------------------------------\n");
    }

}
void sortByTitle() // function that sorts global info array by title from A to Z
{
    int i, pass, swap = 0;
    Book temp;
    for (pass = 0; pass < infoSize - 1; pass++)
    {
        for (i = 0; i < infoSize - pass - 1; i++)
        {
            if (strcmp((info[i].title), info[i + 1].title) == 1)
            {
                temp = info[i];
                info[i] = info[i + 1];
                info[i + 1] = temp;
                swap = 1;
            }
        }
        if (!swap)
            break;
    }
}

void sortByDate () // function that sorts global info array by date from newer to older publications
{
    int i, pass, swap = 0;
    Book temp;
    for (pass = 0; pass < infoSize - 1; pass++)
    {
        for (i = 0; i < infoSize - pass - 1; i++)
        {
            if (info[i].publication.year < info[i + 1].publication.year)
            {
                temp = info[i];
                info[i] = info[i + 1];
                info[i + 1] = temp;
                swap = 1;
            }
            else if (info[i].publication.year == info[i + 1].publication.year)
            {
                if (info[i].publication.month < info[i + 1].publication.month)
                {
                    temp = info[i];
                    info[i] = info[i + 1];
                    info[i + 1] = temp;
                    swap = 1;
                }
            }
            if (!swap)
                break;
        }
    }
}

void sortByPrice () // function that sorts global info array by price from cheaper to more expensive
{
    int i, pass, swap = 0;
    Book temp;
    for (pass = 0; pass < infoSize - 1; pass++)
    {
        for (i = 0; i < infoSize - pass - 1; i++)
        {
            if (info[i].price > info[i + 1].price)
            {
                temp = info[i];
                info[i] = info[i + 1];
                info[i + 1] = temp;
                swap = 1;
            }
        }
        if (!swap)
            break;
    }
}
void advancedSearch (string word)
{
    int i, flag = 0;
    for(i = 0; i < infoSize ; i++)
    {
        if (strstr(info[i].title, word))
        {
            printBook(i);
            flag=1;
        }
    }
    if (flag == 0)
    {
        printf("No matches are found");
    }
}

int isValidISBN (string isbn) // returns 1 if valid 0 if invalid
{
    int i, flag = 1, counter = 0;
    for (i = 0; isbn[i] != '\0'; i++)
    {
        flag = 0;
        if (isbn[i] >= 0 && isbn[i] <= 9)
            counter++;
        else
        {
            flag = 1;
            break;
        }
    }
    if (counter == 13 && !flag)
        return 1;
    else
    {
        printf("Enter valid ISBN!");
        return 0;
    }
}

void modify() // unfinished function
{
    string isbn;
    int check = 1, index = 0, i;
    printf("Enter ISBN of book to modify (ISBN must be 13 digits - All numbers): ");
    do
    {
        scanf("%s", isbn);
        if (isValidISBN(isbn))
            for (i = 0; i < infoSize; i++)
            {
                check = strcmp(isbn, info[i].ISBN);
                if (!check)
                {
                    index = i;
                    break;
                }
            }
    }
    while (!isValidISBN(isbn) && !check);

}


void menu(FILE * fileCredentials, FILE * fileBooks)
{
    char option;

    readCredentials(fileCredentials, numOfLines(fileCredentials) / 2); // calls function to store username and password data into the global credentials array - gives function half number of lines since each element of the credentials array stores 2 lines

    printf("Welcome To our Library System\n");

letter :
    printf("Pleas Enter letter 'l' to login or 'q' to quit : ");
    option = getchar();
    option = tolower(option);


    if(option == 'l')
        login(numofTrials, numOfLines(fileCredentials) / 2);

    else if(option == 'q')
        return;
    else
        goto letter;

    // if the program reach this point that mean the user logged in successfully

    load(fileBooks); // calls function to store books.txt data into the global array "info"
    printf("You can choose one of our options by :\n");
    printf("To ADD enter 'a'\nTo DELETE enter 'd'\nTo MODIFY enter 'm'\nTo SEARCH enter 's'\nTo ADVANCED SEARCH enter 'v'\nTo PRINT enter 'p'\nTo SAVE enter 'z'\nTo QUIT enter 'q'\n");


    printf("\n\nEnter the letter : ");
    option = getchar();

    option = tolower(option);

select :

    if (option == 'a')
        printf("function is not available now\n");

    else if (option == 'd')
        printf("function is not available now\n");
    else if (option == 'm')
        printf("function is not available now\n");
    else if (option == 's')
    {
        string isbn;
        printf("Enter a book ISBN: ");
        scanf("%s",isbn);
        Query(isbn);
    }

    else if (option == 'v')
    {
        string word;
        printf("Enter a subtitle: ");
        scanf("%s",word);
        advancedSearch(word);
    }

    else if (option == 'p')
    {
        //printAll();
        printf("function is not available now\n");
    }

    else if (option == 'z')
        printf("function is not available now\n");
    else if (option == 'q')
        printf("function is not available now\n");

    printf("Do you want to select more options (y,n) ? ");
    scanf("%c", &option);
    option = tolower(option);

    if (option == 'y')
        goto select;
    else
        return;

}

int main()
{

    FILE * fileCredentials = fopen("files//credentials.txt", "r"); // initializes file pointer that opens credentials.txt file to be read only

    if (fileCredentials == NULL)
    {
        printf("Error while opening the file"); // Error message if credentials.txt file is not found
        return 1; // Ends program by returning 1 to show error
    }

    FILE * fileBooks = fopen("files//books.txt", "r+"); // initializes file pointer that opens books.txt file

    if (fileBooks == NULL)
    {
        printf("Error while opening the books.txt file"); // Error message if books.txt file is not found
        return 1; // Ends program by returning 1 to show error
    }

    infoSize = numOfLines(fileBooks); // integer that gets number of lines in books.txt file

    menu(fileCredentials, fileBooks);

    fclose(fileCredentials); // closes credentials.txt file

    fclose(fileBooks); // closes books.txt file

    return 0;
}

