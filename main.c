#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
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
    int i;
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
    return login(--trials, n); // RECURSIVE CASE - in case of incorrect credentials, reduces number of tries remaining by 1 before function calls itself again

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
    printf("Price: $%.2f\n", info[i].price);
    printDate(info[i].publication);
}

int isValidISBN (string isbn) // returns 1 if valid 0 if invalid
{
    int i, flag = 1, counter = 0;
    for (i = 0; isbn[i] != '\0'; i++)
    {
        flag = 0;
        if (isdigit(isbn[i]))
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
        printf("Enter valid ISBN!\n");
    return 0;
}

int isValidMonth (int month)
{
    while (month > 12 || month < 1)
    {
        if (month > 12 || month < 1)
        {
            printf("Enter a valid month(1 to 12): ");
            scanf("%d", &month);
            printf("\n");
        }
    }
    return month;
}

int isValidYear (int year)
{
    while (year > 2022 || year < 0)
    {
        if (year > 2022 || year < 0)
        {
            printf("Enter a valid year(0 to 2022): ");
            scanf("%d", &year);
            printf("\n");
        }
    }
    return year;
}

float isValidPrice (float price)
{
    while (price <= 0)
    {
        printf("Enter a valid price(0 or greater): ");
        scanf("%f", &price);
        printf("\n");
    }
    return price;
}

int isValidQuantity (int quantity)
{
    do
    {
        if (quantity < 0)
        {
            printf("Enter a valid quantity (0 or more): ");
            scanf("%d", &quantity);
        }
    }
    while (quantity < 0);
    return quantity;
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

void advancedSearch (string word)
{
    int i,flag=0;
    for(i=0; i<infoSize; i++)
    {
        if (strstr(info[i].title, word))
        {
            printBook(i);
            flag=1;
        }
    }
    if (flag==0)
    {
        printf("No matches are found");
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

void printAll()
{
    int i, flag = 0, length;
    string sortMethod;
    string title = "title", price = "price", date = "date";
    printf("-----------------------------------------\n");
    printf("Sort by:\nTitle\nPrice\nDate of Publication\n\n");
    printf("-----------------------------------------\n");
    do
    {
        printf("|To choose a sorting method, enter \"TITLE\", \"PRICE\", or \"DATE\"|\n");
        scanf("%s", sortMethod);
        length = strlen(sortMethod);
        for (i = 0; i < length; i++)
        {
            sortMethod[i] = tolower(sortMethod[i]);
        }
        if (!strcmp(title, sortMethod))
        {
            sortByTitle();
            flag = 1;
        }
        else if (!strcmp(price, sortMethod))
        {
            sortByPrice();
            flag = 2;
        }
        else if (!strcmp(date, sortMethod))
        {
            sortByDate();
            flag = 3;
        }
        else
            printf("Enter a valid method!\n");
    }
    while (!flag);
    printf("-----------------------------------------\n");
    for (i = 0; i < infoSize; i++)
    {
        printBook(i);
        printf("-----------------------------------------\n");
    }

}

void modify() // unfinished function
{
    string isbn, yes = "yes", no = "no", answer, inputTitle, inputAuthor;
    int i, found = 1, index = 0, inputmonth, inputyear, inputQuantity;
    float inputprice;
    printf("Enter ISBN of book to modify (ISBN must be 13 digits - All numbers): ");
    do
    {
        gets(isbn);
        if (!isValidISBN(isbn))
            for (i = 0; i < infoSize; i++)
            {
                found = strcmp(isbn, info[i].ISBN);
                if (!found)
                {
                    index = i;
                    break;
                }
            }
    }
    while (!isValidISBN(isbn) && !found);
    printf("Would you like to change the title from %s?\n", info[index].title);
    do
    {
        printf("Enter (YES or NO):\n");
        scanf("%s", answer);
    }
    while (strcasecmp(answer, no) && strcasecmp(answer, yes));
    if (!strcasecmp(answer, yes))
    {
        printf("Enter New Title: ");
        fflush(stdin);
        gets(inputTitle);
        strcpy(info[index].title, inputTitle);
    }
    else if (!strcasecmp(answer, no))
    {
        printf("Title will remain as %s.\n", info[index].title);
    }
    printf("Would you like to change the author's name from %s?\n", info[index].author);
    do
    {
        printf("Enter (YES or NO):\n");
        scanf("%s", answer);
    }
    while (strcasecmp(answer, no) && strcasecmp(answer, yes));
    if (!strcasecmp(answer, yes))
    {
        printf("Enter New Author's name: ");
        fflush(stdin);
        gets(inputAuthor);
        strcpy(info[index].author, inputAuthor);
    }
    else if (!strcasecmp(answer, no))
    {
        printf("Author's name will remain as %s.\n", info[index].author);
    }
    printf("Would you like to change the quantity available from %d?\n", info[index].quantity);
    do
    {
        printf("Enter (YES or NO):\n");
        scanf("%s", answer);
    }
    while (strcasecmp(answer, no) && strcasecmp(answer, yes));
    if (!strcasecmp(answer, yes))
    {
        printf("Enter New Quantity: ");
        scanf("%d", &inputQuantity);
        info[index].quantity = isValidQuantity(inputQuantity);
    }
    else if (!strcasecmp(answer, no))
    {
        printf("Quantity will remain as %d.\n", info[index].quantity);
    }
    printf("Would you like to change the price from $%.2f?\n", info[index].price);
    do
    {
        printf("Enter (YES or NO):\n");
        scanf("%s", answer);
    }
    while (strcasecmp(answer, no) && strcasecmp(answer, yes));
    if (!strcasecmp(answer, yes))
    {
        printf("Enter New Price: ");
        scanf("%f", &inputprice);
        info[index].price = isValidPrice(inputprice);
    }
    else if (!strcasecmp(answer, no))
    {
        printf("Price will remain as $%.2f.\n", info[index].price);
    }
    printf("Would you like to change the date published from %d-%d\n", info[index].publication.month, info[index].publication.year);
    do
    {
        printf("Enter (YES or NO):\n");
        scanf("%s", answer);
    }
    while (strcasecmp(answer, no) && strcasecmp(answer, yes));
    if (!strcasecmp(answer, yes)) // MUST VALIDATE
    {
        printf("Enter New Publication Date Month: ");
        scanf("%d", &inputmonth);
        info[index].publication.month = isValidMonth(inputmonth);
        printf("Enter New Publication Date Year: ");
        scanf("%d", &inputyear);
        info[index].publication.year = isValidYear(inputyear);
    }
    else if (!strcasecmp(answer, no))
    {
        printf("Publication Date will remain as %d-%d.\n", info[index].publication.month, info[index].publication.year);
    }
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

