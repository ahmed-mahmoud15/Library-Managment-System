void addBook()
{
    string isbn;
    int quantity, month, year, isexist = 0, isvalid = 0;
    float price;

    do
    {
        printf("Enter ISBN of new book (ISBN must be 13 digits - All numbers): ");
        fflush(stdin);
        gets(isbn);

        isvalid = isValidISBN(isbn);
        isexist = searchByISBN(isbn);

        if(isvalid && isexist == -1)
            strcpy(info[infoSize].ISBN, isbn);
        else if(isexist != -1)
            printf("\nThis ISBN is already exists\n\n");
    }
    while ( isvalid && isexist != -1 );

    printf("\n\nEnter the book's title : ");
    fflush(stdin);
    gets(info[infoSize].title);

    printf("\n\nEnter the Author's name : ");
    fflush(stdin);
    gets(info[infoSize].author);

    printf("\n\nEnter the Quantity : ");
    scanf("%d", &quantity);
    info[infoSize].quantity = isValidQuantity(quantity);

    printf("\n\nEnter the Price : ");
    scanf("%f", &price);
    info[infoSize].price = isValidPrice(price);

    printf("\n\nEnter New Publication Date Month : ");
    scanf("%d", &month);
    info[infoSize].publication.month = isValidMonth(month);

    printf("\nEnter New Publication Date Year : ");
    scanf("%d", &year);
    info[infoSize].publication.year = isValidYear(year);

    ++infoSize;
}
