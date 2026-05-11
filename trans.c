// ==========================================================
// BANK MANAGEMENT SYSTEM IN C
// Mini Project using File Handling
// ==========================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ================= STRUCTURE =================

struct clientData
{
    int acctNum;
    char firstName[20];
    char lastName[20];
    char phone[15];
    char address[50];
    double balance;
};

// ================= FUNCTION PROTOTYPES =================

void initializeFile();
int login();

void header();
int menu();

void createAccount(FILE *fPtr);
void displayAccount(FILE *fPtr);
void displayAllAccounts(FILE *fPtr);
void depositMoney(FILE *fPtr);
void withdrawMoney(FILE *fPtr);
void updateAccount(FILE *fPtr);
void deleteAccount(FILE *fPtr);
void exportTextFile(FILE *fPtr);

// ================= MAIN FUNCTION =================

int main()
{
    FILE *fPtr;

    initializeFile();

    fPtr = fopen("bank.dat", "rb+");

    if (fPtr == NULL)
    {
        printf("File could not be opened.\n");
        return 1;
    }

    // Login System
    if (!login())
    {
        fclose(fPtr);
        return 0;
    }

    int choice;

    do
    {
        choice = menu();

        switch (choice)
        {
        case 1:
            createAccount(fPtr);
            break;

        case 2:
            displayAccount(fPtr);
            break;

        case 3:
            depositMoney(fPtr);
            break;

        case 4:
            withdrawMoney(fPtr);
            break;

        case 5:
            updateAccount(fPtr);
            break;

        case 6:
            deleteAccount(fPtr);
            break;

        case 7:
            displayAllAccounts(fPtr);
            break;

        case 8:
            exportTextFile(fPtr);
            break;

        case 9:
            printf("\nThank You for Using the System!\n");
            break;

        default:
            printf("\nInvalid Choice!\n");
        }

    } while (choice != 9);

    fclose(fPtr);

    return 0;
}

// ================= INITIALIZE FILE =================

void initializeFile()
{
    FILE *fPtr;

    struct clientData blankClient = {0, "", "", "", "", 0.0};

    fPtr = fopen("bank.dat", "rb");

    if (fPtr == NULL)
    {
        fPtr = fopen("bank.dat", "wb");

        for (int i = 0; i < 100; i++)
        {
            fwrite(&blankClient, sizeof(struct clientData), 1, fPtr);
        }
    }

    fclose(fPtr);
}

// ================= LOGIN SYSTEM =================

int login()
{
    char username[20];
    char password[20];

    printf("\n====================================\n");
    printf("        BANK LOGIN SYSTEM\n");
    printf("====================================\n");

    printf("Username : ");
    scanf("%s", username);

    printf("Password : ");
    scanf("%s", password);

    if (strcmp(username, "admin") == 0 &&
        strcmp(password, "1234") == 0)
    {
        printf("\nLogin Successful!\n");
        return 1;
    }
    else
    {
        printf("\nInvalid Username or Password!\n");
        return 0;
    }
}

// ================= HEADER =================

void header()
{
    printf("\n");
    printf("=========================================\n");
    printf("     BANK MANAGEMENT SYSTEM IN C\n");
    printf("=========================================\n");
}

// ================= MENU =================

int menu()
{
    int choice;

    header();

    printf("1. Create New Account\n");
    printf("2. View Account\n");
    printf("3. Deposit Money\n");
    printf("4. Withdraw Money\n");
    printf("5. Update Account\n");
    printf("6. Delete Account\n");
    printf("7. Display All Accounts\n");
    printf("8. Export Accounts to Text File\n");
    printf("9. Exit\n");

    printf("\nEnter your choice : ");
    scanf("%d", &choice);

    return choice;
}

// ================= CREATE ACCOUNT =================

void createAccount(FILE *fPtr)
{
    struct clientData client = {0, "", "", "", "", 0.0};

    int accountNum;

    printf("\nEnter Account Number (1-100): ");
    scanf("%d", &accountNum);

    if (accountNum < 1 || accountNum > 100)
    {
        printf("Invalid Account Number!\n");
        return;
    }

    fseek(fPtr,
          (accountNum - 1) * sizeof(struct clientData),
          SEEK_SET);

    fread(&client,
          sizeof(struct clientData),
          1,
          fPtr);

    if (client.acctNum != 0)
    {
        printf("Account already exists!\n");
        return;
    }

    client.acctNum = accountNum;

    printf("Enter First Name : ");
    scanf("%s", client.firstName);

    printf("Enter Last Name : ");
    scanf("%s", client.lastName);

    printf("Enter Phone Number : ");
    scanf("%s", client.phone);

    printf("Enter Address : ");
    scanf(" %[^\n]", client.address);

    printf("Enter Initial Balance : ");
    scanf("%lf", &client.balance);

    fseek(fPtr,
          (accountNum - 1) * sizeof(struct clientData),
          SEEK_SET);

    fwrite(&client,
           sizeof(struct clientData),
           1,
           fPtr);

    printf("\nAccount Created Successfully!\n");
}

// ================= VIEW ACCOUNT =================

void displayAccount(FILE *fPtr)
{
    int accountNum;

    struct clientData client;

    printf("\nEnter Account Number : ");
    scanf("%d", &accountNum);

    fseek(fPtr,
          (accountNum - 1) * sizeof(struct clientData),
          SEEK_SET);

    fread(&client,
          sizeof(struct clientData),
          1,
          fPtr);

    if (client.acctNum == 0)
    {
        printf("Account not found!\n");
        return;
    }

    printf("\n====================================\n");

    printf("Account Number : %d\n", client.acctNum);
    printf("Name           : %s %s\n",
           client.firstName,
           client.lastName);

    printf("Phone          : %s\n", client.phone);
    printf("Address        : %s\n", client.address);
    printf("Balance        : %.2lf\n", client.balance);

    printf("====================================\n");
}

// ================= DISPLAY ALL ACCOUNTS =================

void displayAllAccounts(FILE *fPtr)
{
    struct clientData client;

    rewind(fPtr);

    printf("\n============================================================\n");
    printf("ACC NO\tNAME\t\tPHONE\t\tBALANCE\n");
    printf("============================================================\n");

    while (fread(&client,
                 sizeof(struct clientData),
                 1,
                 fPtr))
    {
        if (client.acctNum != 0)
        {
            printf("%d\t%s %s\t%s\t%.2lf\n",
                   client.acctNum,
                   client.firstName,
                   client.lastName,
                   client.phone,
                   client.balance);
        }
    }
}

// ================= DEPOSIT MONEY =================

void depositMoney(FILE *fPtr)
{
    int accountNum;

    double amount;

    struct clientData client;

    printf("\nEnter Account Number : ");
    scanf("%d", &accountNum);

    fseek(fPtr,
          (accountNum - 1) * sizeof(struct clientData),
          SEEK_SET);

    fread(&client,
          sizeof(struct clientData),
          1,
          fPtr);

    if (client.acctNum == 0)
    {
        printf("Account not found!\n");
        return;
    }

    printf("Enter Deposit Amount : ");
    scanf("%lf", &amount);

    client.balance += amount;

    fseek(fPtr,
          -sizeof(struct clientData),
          SEEK_CUR);

    fwrite(&client,
           sizeof(struct clientData),
           1,
           fPtr);

    printf("\nMoney Deposited Successfully!\n");
}

// ================= WITHDRAW MONEY =================

void withdrawMoney(FILE *fPtr)
{
    int accountNum;

    double amount;

    struct clientData client;

    printf("\nEnter Account Number : ");
    scanf("%d", &accountNum);

    fseek(fPtr,
          (accountNum - 1) * sizeof(struct clientData),
          SEEK_SET);

    fread(&client,
          sizeof(struct clientData),
          1,
          fPtr);

    if (client.acctNum == 0)
    {
        printf("Account not found!\n");
        return;
    }

    printf("Enter Withdraw Amount : ");
    scanf("%lf", &amount);

    if (amount > client.balance)
    {
        printf("Insufficient Balance!\n");
        return;
    }

    client.balance -= amount;

    fseek(fPtr,
          -sizeof(struct clientData),
          SEEK_CUR);

    fwrite(&client,
           sizeof(struct clientData),
           1,
           fPtr);

    printf("\nWithdrawal Successful!\n");
}

// ================= UPDATE ACCOUNT =================

void updateAccount(FILE *fPtr)
{
    int accountNum;

    struct clientData client;

    printf("\nEnter Account Number : ");
    scanf("%d", &accountNum);

    fseek(fPtr,
          (accountNum - 1) * sizeof(struct clientData),
          SEEK_SET);

    fread(&client,
          sizeof(struct clientData),
          1,
          fPtr);

    if (client.acctNum == 0)
    {
        printf("Account not found!\n");
        return;
    }

    printf("Enter New Phone Number : ");
    scanf("%s", client.phone);

    printf("Enter New Address : ");
    scanf(" %[^\n]", client.address);

    fseek(fPtr,
          -sizeof(struct clientData),
          SEEK_CUR);

    fwrite(&client,
           sizeof(struct clientData),
           1,
           fPtr);

    printf("\nAccount Updated Successfully!\n");
}

// ================= DELETE ACCOUNT =================

void deleteAccount(FILE *fPtr)
{
    int accountNum;

    struct clientData client;

    struct clientData blankClient = {0, "", "", "", "", 0.0};

    printf("\nEnter Account Number : ");
    scanf("%d", &accountNum);

    fseek(fPtr,
          (accountNum - 1) * sizeof(struct clientData),
          SEEK_SET);

    fread(&client,
          sizeof(struct clientData),
          1,
          fPtr);

    if (client.acctNum == 0)
    {
        printf("Account not found!\n");
        return;
    }

    fseek(fPtr,
          (accountNum - 1) * sizeof(struct clientData),
          SEEK_SET);

    fwrite(&blankClient,
           sizeof(struct clientData),
           1,
           fPtr);

    printf("\nAccount Deleted Successfully!\n");
}

// ================= EXPORT TO TEXT FILE =================

void exportTextFile(FILE *fPtr)
{
    FILE *txtPtr;

    struct clientData client;

    txtPtr = fopen("accounts.txt", "w");

    if (txtPtr == NULL)
    {
        printf("Text file could not be created!\n");
        return;
    }

    rewind(fPtr);

    fprintf(txtPtr,
            "=========================================================\n");

    fprintf(txtPtr,
            "ACCOUNT LIST\n");

    fprintf(txtPtr,
            "=========================================================\n");

    fprintf(txtPtr,
            "ACC NO\tNAME\t\tPHONE\t\tBALANCE\n");

    while (fread(&client,
                 sizeof(struct clientData),
                 1,
                 fPtr))
    {
        if (client.acctNum != 0)
        {
            fprintf(txtPtr,
                    "%d\t%s %s\t%s\t%.2lf\n",
                    client.acctNum,
                    client.firstName,
                    client.lastName,
                    client.phone,
                    client.balance);
        }
    }

    fclose(txtPtr);

    printf("\nAccounts exported to accounts.txt successfully!\n");
}