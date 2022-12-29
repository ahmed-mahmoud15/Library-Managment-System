#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include <unistd.h>
#define MAX 256
#define MIN 50
#define numofTrials 5
#define resetTimer 3

typedef char string[MAX]; // defines a new data type "string" as a character array with "MAX" number of elements

typedef char shortString[MIN]; // defines a new data type "shortString" as a character array with "MIN" number of elements

typedef struct Date // struct that stores the date as an integer month and integer year
{
    int month, year;

} Date;

typedef struct Book
{
    char ISBN[14]; // character array ISBN to store the 13 digit ISBN number as a string with the 14th element being "\0"
    string title;
    shortString author;
    int quantity;
    float price;
    Date publication; // struct publication date that includes month and year
} Book;

typedef struct Account // struct that holds account information (username and password)
{
    shortString username;
    shortString password;
} Account;

Book info[MAX]; // global array of struct Book with up to MAX number of elements
Account Credentials[MAX]; // global array of struct Account with up to MAX number of elements
int infoSize; // global variable for utilized size of info


int numOfLines(FILE *fileCredentials); // function that counts the number of lines in a file

void readCredentials(FILE *fileCredentials, int n); // function that stores user name and password information into the Credentials struct


int login(int trials, int n); // login function that prompts user to enter a username and password and returns 1 if the user inputs valid credentials


void load(FILE * fileBooks); // load function that stores book.txt file data into the global array "info"

void printDate(Date date);

void printBook (int i);

int isValidISBN (shortString isbn); // returns 1 if valid 0 if invalid

int isValidMonth (shortString OLDmonth);

int isValidYear (shortString OLDyear);

float isValidPrice(shortString price);

int isValidQuantity (shortString quantity);

int isStringEmpty(string name);

int searchByISBN(shortString isbn);

void query ();

void advancedSearch ();

void sortByTitle(); // function that sorts global info array by title from A to Z

void sortByDate (); // function that sorts global info array by date from newer to older publications

void sortByPrice (); // function that sorts global info array by price from cheaper to more expensive

void printTable();

void printAll();

void addBook();

void modify();

void deleteBook ();

void save();

void quit();

void chooseOption();

void menu(FILE * fileCredentials, FILE * fileBooks);

int main()
{

    FILE * fileCredentials = fopen("files//credentials.txt", "r"); // initializes file pointer that opens credentials.txt file to be read only

    if (fileCredentials == NULL)
    {
        printf("Error while opening the file: \"credentials.txt\"!"); // Error message if credentials.txt file is not found
        return 1; // Ends program by returning 1 to show error
    }

    FILE * fileBooks = fopen("files//books.txt", "r"); // initializes file pointer that opens books.txt file

    if (fileBooks == NULL)
    {
        printf("Error while opening the file: \"books.txt\"!"); // Error message if books.txt file is not found
        return 2; // Ends program by returning 2 to show error
    }


    infoSize = numOfLines(fileBooks); // integer that gets number of lines in books.txt file

    menu(fileCredentials, fileBooks);

    fclose(fileCredentials); // closes credentials.txt file

    fclose(fileBooks); // closes books.txt file

    return 0;
}

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
    int i,j;
    char option;

    if (trials == 0) // condition that checks whether the user used up the limited number of trials
    {
        printf("Wait 10 seconds to try again\n");
        sleep(10);

        printf("\nDo you want to login again -enter \'l\'- ? else quit :: ");
        scanf("%c", &option);
        option = tolower(option);

        if(option == 'l')
            return 0;

        else
            exit(0);

    }
    string user, pass;

    fflush(stdin);
    printf("\nEnter Username: ");
    gets(user);

    fflush(stdin);
    printf("Enter Password: ");
    gets(pass);

    for(i =0; i < n; i++) // for loop that passes through the Credentials array
        if (strcmp(Credentials[i].username, user) == 0 && strcmp(Credentials[i].password, pass) == 0) // BASE CASE - condition to check if the user inputted username and password match with any username and password from credentials array
        {
            printf("\n\nWelcome %s :D\n", Credentials[i].username);
            printf("Your login is processing");

            for(j = resetTimer; j != 0 ; j--)
            {
                printf(".");
                sleep(1);
            }

            return 1; // leaves function by returning 1 and skips the remaining lines of the function without executing
        }
    printf("Invalid username or password\nYou have %d more trials\n\n\n", trials-1); //prints error message informing the user of how many tries they have left before the program closes
    return login(--trials, n); // RECURSIVE CASE - in case of incorrect credentials, reduces number of tries remaining by 1 before function calls itself again

}

void load(FILE * fileBooks) // load function that stores book.txt file data into the global array "info"
{
    rewind(fileBooks); // function repositions the file pointer to the beginning of the file
    string temp;
    int i;
    for (i=0; i < infoSize; i++)
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
    shortString months[12] =
    {
        "January", "February", "March", "April", "May", "June",
        "July", "August", "September", "October", "November", "December"
    };
    printf("%s %d", months[date.month - 1], date.year);
}

void printBook (int i)
{
    printf("\nISBN: %s\n", info[i].ISBN);
    printf("Title: %s\n", info[i].title);
    printf("Author: %s\n", info[i].author);
    printf("Quantity: %d\n", info[i].quantity);
    printf("Price: $%.2f\n", info[i].price);
    printf("Published: ");
    printDate(info[i].publication);
    printf("\n\n");
}

int isValidISBN (shortString isbn) // returns 1 if valid 0 if invalid
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
        printf("Enter valid ISBN: ");
    return 0;
}

int isValidMonth (shortString OLDmonth)
{
    int i, valid, count, validmonth;
    do
    {
        valid = 0;
        i = 0;
        count = 0;
        while (OLDmonth[i] != '\0')
        {
            count++;
            if (!isdigit(OLDmonth[i]))
            {
                valid = 0;
                fflush(stdin);
                printf("Enter a valid month (whole number from 1 to 12): ");
                scanf("%s", OLDmonth);
                break;
            }
            else
                valid = 1;
            i++;
        }
        if (valid)
            validmonth = atoi(OLDmonth);

        if (validmonth > 12 || validmonth < 1)
        {
            printf("Enter a valid month (whole number from 1 to 12): ");
            scanf("%s", OLDmonth);
        }
    }
    while (!valid || !count || validmonth > 12 || validmonth < 1);
    return validmonth;
}

int isValidYear (shortString OLDyear)
{
    int i, valid, count, validyear;
    do
    {
        valid = 0;
        i = 0;
        count = 0;
        while (OLDyear[i] != '\0')
        {
            count++;
            if (!isdigit(OLDyear[i]))
            {
                valid = 0;
                fflush(stdin);
                printf("Enter a valid year (whole number 0 to 2022): ");
                scanf("%s", OLDyear);
                break;
            }
            else
                valid = 1;
            i++;
        }
        if (valid)
            validyear = atoi(OLDyear);

        if (validyear > 2022 || validyear < 0)
        {
            printf("Enter a valid year (whole number 0 to 2022): ");
            scanf("%s", OLDyear);
        }
    }
    while (!valid || !count || validyear > 2022 || validyear < 0);
    return validyear;
}

float isValidPrice(shortString price)
{
    int i, valid, count, pointcounter;
    float validprice;
    do
    {
        valid = 0;
        i = 0;
        count = 0;
        pointcounter = 0;
        while (price[i] != '\0')
        {
            count++;
            if (price[i] == '.')
                ++pointcounter;

            if ((!isdigit(price[i]) && price[i] != '.') || pointcounter > 1)
            {
                valid = 0;
                printf("Enter a valid price (whole or decimal number ex. 2.15): ");
                scanf("%s", price);
                break;
            }
            else
                valid = 1;
            i++;
        }
        if (valid && (!pointcounter || pointcounter == 1))
            validprice = atof(price);

    }
    while (!valid || !count || pointcounter > 1);
    return validprice;
}

int isValidQuantity (shortString quantity)
{
    int i, valid, count, validquantity;
    do
    {
        valid = 0;
        i = 0;
        count = 0;
        while (quantity[i] != '\0')
        {
            count++;
            if (!isdigit(quantity[i]))
            {
                valid = 0;
                printf("Enter a valid quantity (whole number greater than or equal to 0 ex. 4): ");
                scanf("%s", quantity);
                break;
            }
            else
                valid = 1;
            i++;
        }
        if (valid)
            validquantity = atoi(quantity);
    }
    while (!valid || !count);
    return validquantity;
}

int isStringEmpty(string name)
{
    int s;

    for(s = 0; isspace(name[s]) ; s++);

    while(1)
    {
        if( strlen(name) == s )
        {
            printf("Enter valid name : ");
            return 0;
        }
        else
            return  1;
    }

}

int searchByISBN(shortString isbn)
{
    int i;
    for(i = 0; i < infoSize; i++)
    {
        if (strcmp(isbn, info[i].ISBN) == 0)
        {
            return i;
        }
    }
    return -1;
}

void query ()
{
    shortString isbn;
    int isValid = 0;
    char option;

    printf("Enter a valid book ISBN: ");
    do
    {
        scanf("%s", isbn);
        isValid = isValidISBN(isbn);
    }
    while(!isValid);
    int index = searchByISBN(isbn);
    if (index > -1)
    {
        printBook(index);
    }
    else
    {
        printf("The ISBN entered is not available\n");
    }

    printf("\ndo you want to search for another book (y,n)? :");
    do
    {
        fflush(stdin);
        scanf("%c", &option);
        option = tolower(option);

        if(option == 'y')
        {
            system("cls");
            query();
        }
        else if(option == 'n')
            return;
        else
            printf("Invalid input!\nenter 'y' or 'n' :");
    }
    while(option != 'y' && option != 'n');

}

void advancedSearch ()
{
    string word, temp;
    char option;

    printf("\nEnter a book title: ");
    scanf("%s", word);
    int i,flag=0;

    strcpy(word, strlwr(word));

    for(i=0; i<infoSize; i++)
    {

        strcpy(temp, info[i].title);
        strcpy(temp, strlwr(temp));

        if (strstr(temp, word))
        {
            printBook(i);
            flag=1;
        }
    }
    if (flag==0)
    {
        printf("No matches are found\n");
    }

    printf("\ndo you want to Search for another book (y,n)? :");
    do
    {
        fflush(stdin);
        scanf("%c", &option);
        option = tolower(option);

        if(option == 'y')
        {
            system("cls");
            advancedSearch();
        }
        else if(option == 'n')
            return;
        else
            printf("Invalid input!\nenter 'y' or 'n' : ");
    }
    while(option != 'y' && option != 'n');

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

void printTable()
{
    int i;
    printf("  # | %-13s | %-50s | %-30s | %-6s | %-8s | %s \n", "ISBN", "Title", "Author Name", "Price", "Quantity", "Publication Date");
    for (i = 0; i < 150; i++)
    {
        printf("-");
    }
    printf("\n");
    for (i= 0; i < infoSize; i++)
    {
        shortString price, quantity;
        snprintf(quantity, 8, "%d", info[i].quantity);
        snprintf(price, 6, "%.2f", info[i].price);
        printf(" %02d | %-13s | %-50s | %-30s | $%-6s | %-8s | ", i+1, info[i].ISBN, info[i].title, info[i].author, price, quantity);
        printDate(info[i].publication);
        printf("\n");
    }
}
void printAll()
{
    int flag = 0;
    shortString sortMethod;
    shortString title = "title", price = "price", date = "date";
    printf("-----------------------------------------\n\n");
    printf("Sort by : \nTitle \nPrice \nDate of Publication");
    printf("\n\n-----------------------------------------\n");
    do
    {
        printf("\nChoose a sorting method\n"
               "Enter \"TITLE\", \"PRICE\", or \"DATE\"\n");
        scanf("%s", sortMethod);
        printf("\n");
        if (!strcasecmp(title, sortMethod))
        {
            sortByTitle();
            flag = 1;
        }
        else if (!strcasecmp(price, sortMethod))
        {
            sortByPrice();
            flag = 2;
        }
        else if (!strcasecmp(date, sortMethod))
        {
            sortByDate();
            flag = 3;
        }
        else
            printf("Enter a valid method!\n");
    }
    while (!flag);

    system("cls");
    printTable();

}

void addBook()
{
    shortString isbn, inputQuantity, inputprice, inputmonth, inputyear, author;
    string title;

    int index = 0, isValid = 0;
    char option;
    printf("\nEnter ISBN of the book to be added (ISBN must be 13 digits - All numbers) : ");
    do
    {
        fflush(stdin);
        gets(isbn);
        isValid = isValidISBN(isbn);

        index = searchByISBN(isbn);

        if(index != -1)
        {
            printf("\nThis Book already exists!\n");
            printf("Enter \"n\" to enter new ISBN\n"
                   "Enter \"m\" to choose other option");
            do
            {
                fflush(stdin);
                printf("\nEnter \"n\" or \"m\" : \n");
                scanf("%c", &option);
                option = tolower(option);
                if(option == 'n')
                    printf("Enter another ISBN : \n");
                else if(option == 'm')
                    return;
                else
                    printf("Invalid input!\nEnter 'n' to enter new ISBN\n"
                           "Enter 'm' to choose other option\n");
            }
            while (option != 'n' && option != 'm');
        }
    }
    while(!isValid || index  != -1);
    strcpy(info[infoSize].ISBN, isbn);

    printf("\nEnter the Title : ");
    do
    {
        fflush(stdin);
        gets(title);
    }
    while(isStringEmpty(title) == 0);
    strcpy(info[infoSize].title, title);


    printf("\nEnter the Author Name : ");
    do
    {
        fflush(stdin);
        gets(author);
    }
    while(isStringEmpty(author) == 0);
    strcpy(info[infoSize].author, author);


    printf("\nEnter the Quantity(whole number ex. 4): ");
    scanf("%s", inputQuantity);
    info[infoSize].quantity = isValidQuantity(inputQuantity);

    printf("\nEnter the price(decimal number ex. 2.15): ");
    scanf("%s", inputprice);
    info[infoSize].price = isValidPrice(inputprice);

    printf("\nEnter the Publication Date Month(whole number 1 to 12): ");
    scanf("%s",inputmonth);
    info[infoSize].publication.month = isValidMonth(inputmonth);

    printf("\nEnter the Publication Date Year(whole number 0 to 2022): ");
    scanf("%s",inputyear);
    info[infoSize].publication.year = isValidYear(inputyear);

    printf("\nThe new book had been added successfully. :)\n");
    printBook(infoSize);
    infoSize++;
    printf("\nBooks information after Addition : \n\n");
    printTable();

    printf("\ndo you want to add another book (y,n)? :");
    do
    {
        fflush(stdin);
        scanf("%c", &option);
        option = tolower(option);

        if(option == 'y')
        {
            system("cls");
            addBook();
        }
        else if(option == 'n')
            return;
        else
            printf("Invalid input!\nenter 'y' or 'n' : ");
    }
    while(option != 'y' && option != 'n');

}

void modify()
{
    shortString isbn, yes = "yes", no = "no", answer, inputAuthor, inputQuantity, inputprice, inputmonth, inputyear;
    string inputTitle;
    int index = 0, isValid = 0;
    char option;

    printf("\nEnter ISBN of book to modify (ISBN must be 13 digits - All numbers): ");
    do
    {
        fflush(stdin);
        gets(isbn);
        isValid = isValidISBN(isbn);
        if (isValid)
        {
            index = searchByISBN(isbn);
            if (index == -1)
            {
                printf("This ISBN is not found. Enter another one: ");
            }
        }
    }
    while (!isValid || index == -1);
    printf("\nWould you like to change the title from %s?\n", info[index].title);
    do
    {
        printf("Enter (YES or NO) : ");
        scanf("%s", answer);
    }
    while (strcasecmp(answer, no) && strcasecmp(answer, yes));
    if (!strcasecmp(answer, yes))
    {
        printf("\nEnter New Title: ");

        do
        {
            fflush(stdin);
            gets(inputTitle);
        }
        while(isStringEmpty(inputTitle) == 0);
        strcpy(info[index].title, inputTitle);

    }
    else if (!strcasecmp(answer, no))
    {
        printf("\nTitle will remain as %s.\n\n", info[index].title);
    }
    printf("\nWould you like to change the author's name from %s?\n", info[index].author);
    do
    {
        printf("Enter (YES or NO) : ");
        scanf("%s", answer);
    }
    while (strcasecmp(answer, no) && strcasecmp(answer, yes));
    if (!strcasecmp(answer, yes))
    {
        printf("\nEnter New Author's name: ");
        do
        {
            fflush(stdin);
            gets(inputAuthor);
        }
        while(isStringEmpty(inputAuthor) == 0);
        strcpy(info[index].author, inputAuthor);
    }
    else if (!strcasecmp(answer, no))
    {
        printf("\nAuthor's name will remain as %s.\n\n", info[index].author);
    }
    printf("\nWould you like to change the quantity available from %d?\n", info[index].quantity);
    do
    {
        printf("Enter (YES or NO) : ");
        scanf("%s", answer);
    }
    while (strcasecmp(answer, no) && strcasecmp(answer, yes));
    if (!strcasecmp(answer, yes))
    {
        printf("\nEnter New Quantity(whole number ex. 4): ");
        scanf("%s", inputQuantity);
        info[index].quantity = isValidQuantity(inputQuantity);
    }
    else if (!strcasecmp(answer, no))
    {
        printf("\nQuantity will remain as %d.\n\n", info[index].quantity);
    }
    printf("\nWould you like to change the price from $%.2f?\n", info[index].price);
    do
    {
        printf("Enter (YES or NO) : ");
        scanf("%s", answer);
    }
    while (strcasecmp(answer, no) && strcasecmp(answer, yes));
    if (!strcasecmp(answer, yes))
    {
        printf("Enter New Price(decimal number ex. 2.15): ");
        scanf("%s", inputprice);
        info[index].price = isValidPrice(inputprice);
    }
    else if (!strcasecmp(answer, no))
    {
        printf("\nPrice will remain as $%.2f.\n\n", info[index].price);
    }
    printf("\nWould you like to change the date published from %d-%d\n", info[index].publication.month, info[index].publication.year);
    do
    {
        printf("Enter (YES or NO) : ");
        scanf("%s", answer);
    }
    while (strcasecmp(answer, no) && strcasecmp(answer, yes));
    if (!strcasecmp(answer, yes)) // MUST VALIDATE
    {
        printf("\nEnter New Publication Date Month: ");
        scanf("%s", inputmonth);
        info[index].publication.month = isValidMonth(inputmonth);
        printf("Enter New Publication Date Year: ");
        scanf("%s", inputyear);
        info[index].publication.year = isValidYear(inputyear);
    }
    else if (!strcasecmp(answer, no))
    {
        printf("\nPublication Date will remain as %d-%d.\n\n", info[index].publication.month, info[index].publication.year);
    }

    printf("\ndo you want to Modify another book (y,n)? :");
    do
    {
        fflush(stdin);
        scanf("%c", &option);
        option = tolower(option);

        if(option == 'y')
        {
            system("cls");
            modify();
        }
        else if(option == 'n')
            return;
        else
            printf("Invalid input!\nenter 'y' or 'n' : ");
    }
    while(option != 'y' && option != 'n');
}

void deleteBook ()
{
    shortString isbn;
    int index = 0, valid = 0;
    char option;
    Book temp;

    printf("\nEnter ISBN of book to delete(ISBN must be 13 digits - All numbers): ");
    do
    {
        fflush(stdin);
        gets(isbn);
        valid = isValidISBN(isbn);

        if (valid)
        {
            index = searchByISBN(isbn);
            if (index == -1)
            {
                printf("This ISBN is not found!\n");
                printf("\nEnter 'd' to enter another ISBN\n"
                       "Enter 'm' to go back to menu");

                fflush(stdin);
                printf("\n\nEnter the letter : ");
                scanf("%c", &option);
                option = tolower(option);
                if(option == 'd')
                    printf("\nEnter another ISBN :");
                else if(option == 'm')
                    return;
            }

        }
    }
    while (!valid || index == -1);

    temp = info[index];
    info[index] = info[infoSize - 1];
    info[infoSize - 1] = temp;
    --infoSize;
    printf("\nBooks information after Deletion : \n\n");
    printTable();

    printf("\ndo you want to Delete another book (y,n)? :");
    do
    {
        fflush(stdin);
        scanf("%c", &option);
        option = tolower(option);

        if(option == 'y')
        {
            system("cls");
            deleteBook();
        }
        else if(option == 'n')
            return;
        else
            printf("Invalid input!\nenter 'y' or 'n' : ");
    }
    while(option != 'y' && option != 'n');
}

void save()
{
    int i;
    FILE * fileBooks = fopen("files//books.txt", "w"); // initializes file pointer that opens books.txt file

    if (fileBooks == NULL)
    {
        printf("Error while opening the books.txt file"); // Error message if books.txt file is not found
        return;
    }

    for(i = 0; i < infoSize ; i++)
    {
        fprintf(fileBooks, "%s",info[i].ISBN);
        fprintf(fileBooks, ",%s",info[i].title);
        fprintf(fileBooks, ",%s",info[i].author);
        fprintf(fileBooks, ",%d",info[i].quantity);
        fprintf(fileBooks, ",%.2f",info[i].price);

        if(i == infoSize - 1)
            fprintf(fileBooks, ",%d-%d",info[i].publication.month, info[i].publication.year);
        else
            fprintf(fileBooks, ",%d-%d\n",info[i].publication.month, info[i].publication.year);
    }
    fclose(fileBooks);
}

void quit()
{
    system("cls");
    char option;

    printf("\aAre you sure you want to exit the program ? (y/n) ");
    fflush(stdin);
    scanf("%c", &option);
    option = tolower(option);

    if(option == 'y')
    {
        printf("\n\nWould you like to save changes -IF ANY- (y/n) ");
        fflush(stdin);
        scanf("%c", &option);
        option = tolower(option);

        if(option == 'y')
            save();

        printf("\nGoodbye, have a nice day! :D\n");
        exit(0);

    }
    else
        return;
}

void chooseOption()
{
    int i;
    char option;
    system("cls");
    printf("You can choose one of our options by :\n\n");
    printf("### To\tADD             enter 'a'\n"
           "### To\tDELETE          enter 'd'\n"
           "### To\tMODIFY          enter 'm'\n"
           "### To\tSEARCH          enter 's'\n"
           "### To\tADVANCED SEARCH enter 'v'\n"
           "### To\tPRINT           enter 'p'\n"
           "### To\tSAVE            enter 'z'\n"
           "### To\tQUIT            enter 'q'\n");

    fflush(stdin);
    printf("\n\nEnter the letter : ");
    scanf("%c", &option);
    option = tolower(option);
    if (option == 'a')
        addBook();

    else if (option == 'd')
        deleteBook();

    else if (option == 'm')
        modify();

    else if (option == 's')
        query();

    else if (option == 'v')
        advancedSearch();

    else if (option == 'p')
        printAll();

    else if (option == 'z')
        save();

    else if (option == 'q')
        quit();

    else
    {
        printf("\nIncorrect Input!\n");
        printf("Try again in : ");
        for (i = resetTimer; i != 0; i--)
        {
            printf("%d", i);
            sleep(1);
            printf("\b");
        }
        chooseOption();
    }
}

void menu(FILE * fileCredentials, FILE * fileBooks)
{
    int flag;
    char option;

    readCredentials(fileCredentials, numOfLines(fileCredentials) / 2); // calls function to store username and password data into the global credentials array - gives function half number of lines since each element of the credentials array stores 2 lines
    printf("=======================================\n\n");
    printf("Welcome To our Library System :)");
    printf("\n\n=======================================\n\n");

    /*while(1)
    {
        fflush(stdin);
        printf("Please Enter letter 'l' to login or 'q' to quit : ");
        scanf("%c", &option);
        option = tolower(option);
        if(option == 'l')
        {
            do
            {
                flag = login(numofTrials, numOfLines(fileCredentials) / 2);
            }
            while (flag == 0);
            break;
        }

        else if(option == 'q')
        {
            printf("\nGoodbye, have a nice day! :D\n");
            exit(0);
        }
        else
            continue;
    }*/

    // if the program reach this point that mean the user logged in successfully
    load(fileBooks); // calls function to store books.txt data into the global array "info"

    while(1)
    {
        chooseOption();

        printf("\nDo you want to select another option (y,n) ? ");
        fflush(stdin);
        scanf("%c", &option);
        option = tolower(option);
        if (option == 'y')
            continue;
        else if(option == 'n')
            quit();
        else
            continue;
    }
}
