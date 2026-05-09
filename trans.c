
#include <stdio.h>
#include <stdlib.h>

struct clientData
{
    unsigned int acctNum;
    char lastName[15];
    char firstName[10];
    double balance;
};

// function prototypes
unsigned int enterChoice(void);
void textFile(FILE *readPtr);
void updateRecord(FILE *fPtr);
void newRecord(FILE *fPtr);
void deleteRecord(FILE *fPtr);
void displayAll(FILE *fPtr);
void searchRecord(FILE *fPtr);
int accountExists(FILE *fPtr, unsigned int acc);

int main()
{
    FILE *cfPtr;
    unsigned int choice;

    if ((cfPtr = fopen("credit.dat", "rb+")) == NULL)
    {
        printf("File could not be opened.\n");
        exit(1);
    }

    while ((choice = enterChoice()) != 7)
    {
        switch (choice)
        {
        case 1:
            textFile(cfPtr);
            break;
        case 2:
            updateRecord(cfPtr);
            break;
        case 3:
            newRecord(cfPtr);
            break;
        case 4:
            deleteRecord(cfPtr);
            break;
        case 5:
            displayAll(cfPtr);
            break;
        case 6:
            searchRecord(cfPtr);
            break;
        default:
            printf("Invalid choice\n");
        }
    }

    fclose(cfPtr);
    return 0;
}

// MENU
unsigned int enterChoice(void)
{
    unsigned int choice;

    printf("\n1 - Store accounts in text file\n");
    printf("2 - Update account\n");
    printf("3 - Add new account\n");
    printf("4 - Delete account\n");
    printf("5 - Display all accounts\n");
    printf("6 - Search account\n");
    printf("7 - Exit\n");
    printf("Enter choice: ");

    scanf("%u", &choice);
    return choice;
}

// DISPLAY ALL
void displayAll(FILE *fPtr)
{
    struct clientData client = {0};

    rewind(fPtr);

    printf("\n%-6s%-15s%-10s%10s\n", "Acct", "Last Name", "First Name", "Balance");

    while (fread(&client, sizeof(struct clientData), 1, fPtr))
    {
        if (client.acctNum != 0)
        {
            printf("%-6d%-15s%-10s%10.2f\n",
                   client.acctNum, client.lastName,
                   client.firstName, client.balance);
        }
    }
}

// SEARCH
void searchRecord(FILE *fPtr)
{
    unsigned int acc;
    struct clientData client;

    printf("Enter account number to search: ");
    scanf("%u", &acc);

    fseek(fPtr, (acc - 1) * sizeof(struct clientData), SEEK_SET);
    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum == 0)
    {
        printf("Account not found.\n");
    }
    else
    {
        printf("\nAccount Found:\n");
        printf("%-6d%-15s%-10s%10.2f\n",
               client.acctNum, client.lastName,
               client.firstName, client.balance);
    }
}

// CHECK ACCOUNT EXISTS
int accountExists(FILE *fPtr, unsigned int acc)
{
    struct clientData client;

    fseek(fPtr, (acc - 1) * sizeof(struct clientData), SEEK_SET);
    fread(&client, sizeof(struct clientData), 1, fPtr);

    return client.acctNum != 0;
}

// UPDATE
void updateRecord(FILE *fPtr)
{
    unsigned int account;
    double transaction;
    struct clientData client;

    printf("Enter account number: ");
    scanf("%u", &account);

    if (!accountExists(fPtr, account))
    {
        printf("Account does not exist.\n");
        return;
    }

    fseek(fPtr, (account - 1) * sizeof(struct clientData), SEEK_SET);
    fread(&client, sizeof(struct clientData), 1, fPtr);

    printf("Current balance: %.2f\n", client.balance);
    printf("Enter amount (+ deposit / - withdraw): ");
    scanf("%lf", &transaction);

    client.balance += transaction;

    fseek(fPtr, -sizeof(struct clientData), SEEK_CUR);
    fwrite(&client, sizeof(struct clientData), 1, fPtr);

    printf("Updated successfully!\n");
}

// ADD NEW
void newRecord(FILE *fPtr)
{
    struct clientData client = {0};
    unsigned int acc;

    printf("Enter new account number: ");
    scanf("%u", &acc);

    if (accountExists(fPtr, acc))
    {
        printf("Account already exists.\n");
        return;
    }

    printf("Enter lastname firstname balance: ");
    scanf("%14s%9s%lf", client.lastName, client.firstName, &client.balance);

    client.acctNum = acc;

    fseek(fPtr, (acc - 1) * sizeof(struct clientData), SEEK_SET);
    fwrite(&client, sizeof(struct clientData), 1, fPtr);

    printf("Account created!\n");
}

// DELETE
void deleteRecord(FILE *fPtr)
{
    unsigned int acc;
    struct clientData blank = {0};

    printf("Enter account number to delete: ");
    scanf("%u", &acc);

    if (!accountExists(fPtr, acc))
    {
        printf("Account does not exist.\n");
        return;
    }

    fseek(fPtr, (acc - 1) * sizeof(struct clientData), SEEK_SET);
    fwrite(&blank, sizeof(struct clientData), 1, fPtr);

    printf("Account deleted successfully.\n");
}

// TEXT FILE
void textFile(FILE *readPtr)
{
    FILE *writePtr;
    struct clientData client;

    if ((writePtr = fopen("accounts.txt", "w")) == NULL)
    {
        printf("File cannot be opened.\n");
        return;
    }

    rewind(readPtr);

    fprintf(writePtr, "%-6s%-15s%-10s%10s\n",
            "Acct", "Last Name", "First Name", "Balance");

    while (fread(&client, sizeof(struct clientData), 1, readPtr))
    {
        if (client.acctNum != 0)
        {
            fprintf(writePtr, "%-6d%-15s%-10s%10.2f\n",
                    client.acctNum, client.lastName,
                    client.firstName, client.balance);
        }
    }

    fclose(writePtr);
    printf("Data written to accounts.txt\n");
}