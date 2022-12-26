#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 100

typedef char string[MAX];

typedef struct Date
{
    int month, year;

} Date;

typedef struct Book
{
    char ISBN[14];
    string name;
    string author;
    int quantity;
    float price;
    Date publication;
} Book;

typedef struct Account
{
    string username;
    string password;
} Account;

Book info[MAX];
Account Credentials[MAX];

int numOfLines(FILE *fileCredentials)
{
    rewind(fileCredentials);
    int count = 1;
    char ch;

    while( (ch = fgetc(fileCredentials)) != EOF)
        if( ch == '\n')
            count++;
    return count;
}

void readCredentials(FILE *fileCredentials, int n)
{
    rewind(fileCredentials);
    int i,j;
    for(i =0 ; !feof(fileCredentials) ; i++)
    {
        fgets(Credentials[i].username, sizeof(Credentials[i].username), fileCredentials);
        Credentials[i].username[strlen(Credentials[i].username)-1] = '\0';
        fgets(Credentials[i].password, sizeof(Credentials[i].password), fileCredentials);
        Credentials[i].password[strlen(Credentials[i].password)-1] = '\0';
    }

}

int login(int trials, int n)
{
    int i;
    if (trials == 0)
        return 0;

    string user, pass;
    printf("Enter Username: ");
    gets(user);
    printf("Enter Password: ");
    gets(pass);
    for(i =0; i < n; i++)
        if (strcmp(Credentials[i].username, user) == 0 && strcmp(Credentials[i].password, pass) == 0)
            return 1;

    printf("Wrong username or password\nyou have %d more trials\n\n\n",--trials);
    login(trials, n);

}

void load(FILE * fileBooks, int n, Book books[n])
{
    rewind(fileBooks);
    string temp;
    int i;
    for (i=0; i< n; i++)
    {
        fgets(temp, MAX, fileBooks);

        strcpy(books[i].ISBN, strtok(temp, ","));
        strcpy(books[i].name, strtok(NULL, ","));
        strcpy(books[i].author, strtok(NULL, ","));

        books[i].quantity = atoi(strtok(NULL, ","));
        books[i].price = atof(strtok(NULL, ","));
        books[i].publication.month = atoi(strtok(NULL, "-"));
        books[i].publication.year = atoi(strtok(NULL, "-"));
    }
}

int main()
{
    int i, nLines;

    FILE * fileCredentials = fopen("files//credentials.txt", "r");
    FILE * fileBooks = fopen("files//books.txt", "r");
    if (fileCredentials == NULL)
    {
        printf("Error while opening the file");
        return 1;
    }
    if (fileBooks == NULL)
    {
        printf("Error while opening the file");
        return 1;
    }
    nLines = numOfLines(fileCredentials);

    readCredentials(fileCredentials, nLines / 2);

    if(login(5, nLines / 2))
        printf("DONE\n");
    else
        return 1;

    nLines = numOfLines(fileBooks);

    Book books[nLines];
    load(fileBooks, nLines, books);

    return 0;
}


