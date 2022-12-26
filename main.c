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
        fgets(Credentials[i].username, MAX, fileCredentials);
        fgets(Credentials[i].password, MAX, fileCredentials);
    }
    string tempuser, temppass, newtemp, NEWERtemp;
    for (i = 0; i < n; i++)
    {
        j=0;
        strcpy(tempuser, Credentials[i].username);
        while (tempuser[j] != '\n')
        {
            newtemp[j] = tempuser[j];
            j++;
        }
        newtemp[j]= '\0';
        strcpy(Credentials[i].username, newtemp);
        j = 0;
        strcpy(temppass, Credentials[i].password);
        while (temppass[j] != '\n')
        {
            NEWERtemp[j] = temppass[j];
            j++;
        }
        NEWERtemp[j]= '\0';
        strcpy(Credentials[i].password, NEWERtemp);

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

int main()
{
    int i, nLines;

    FILE * fileCredentials = fopen("files//credentials.txt", "r");
    if (fileCredentials == NULL)
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



    return 0;
}


