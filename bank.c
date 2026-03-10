#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef _WIN32
#include <conio.h>
#define CLEAR "cls"
#else
#include <termios.h>
#include <unistd.h>
#define CLEAR "clear"

int getch(void) {
    struct termios oldt, newt;
    int ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}
#endif

#define RED   "\x1B[31m"
#define GREEN "\x1B[32m"
#define BLUE  "\x1B[34m"
#define CYAN  "\x1B[36m"
#define RESET "\x1B[0m"

#define MAX_ACCOUNTS 100

typedef struct {
    int account_number;
    char account_holder[50];
    char password[50];
    int pin;
    float balance;
} BankAccount;

/* Global state */
BankAccount acc[MAX_ACCOUNTS];
int num = 0;
int count = 0;
int current_user_index = -1;

/* Function prototypes */
void divider(void);
void clearAccountData(void);
void loginMenu(void);
void homeMenu(void);
void createAccount(void);
void deposit(void);
void withdraw(void);
void transfer(void);
void display(void);
void transactionDetail(void);
void adminMenu(void);
void allTransactionDetails(void);
void confidential(void);
void deleteAccount(void);
void logout(void);
void mainMenu(void);

void divider(void) {
    int i;
    for (i = 0; i < 40; i++) printf("-");
    printf("\n");
}

void clearAccountData(void) {
    FILE *f1 = fopen("bank.txt", "w");
    if (f1) fclose(f1);
    FILE *f2 = fopen("account.txt", "w");
    if (f2) fclose(f2);
}

void mainMenu(void) {
    while (1) {
        loginMenu();
        if (current_user_index != -1) {
            homeMenu();
        }
    }
}

void loginMenu(void) {
    int choice;

    while (1) {
        system(CLEAR);
        printf(CYAN "\n BANK MANAGEMENT SYSTEM \n" RESET);
        divider();
        printf(GREEN "\n1. Login\n");
        printf("2. Sign Up\n" BLUE);
        printf("3. Admin Login\n" RED);
        printf("4. Exit\n" RESET);

        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1: {
            system(CLEAR);
            int attempts;
            for (attempts = 0; attempts < 3; attempts++) {
                int acc_num;
                char acc_pass[50];
                char ch;
                printf(CYAN "\nLOGIN\n" RESET);
                divider();
                printf("\n");
                printf(RED "ONLY %d ATTEMPTS!\n" RESET, 3 - attempts);
                printf("\nEnter account number: ");
                scanf("%d", &acc_num);
                printf("Enter password: ");
                scanf(" ");
                fgets(acc_pass, 50, stdin);
                acc_pass[strcspn(acc_pass, "\n")] = '\0';
                printf("Press 'y' to continue and 'n' to go to main menu: ");
                scanf(" %c", &ch);
                if (ch == 'n') break;

                int i;
                for (i = 0; i < num; i++) {
                    if (acc[i].account_number == acc_num && strcmp(acc[i].password, acc_pass) == 0) {
                        current_user_index = i;
                        return;
                    }
                }
                printf(RED "Invalid credentials. Try again.\n" RESET);
            }
            printf(RED "Too many failed attempts. Returning to main menu.\n" RESET);
            break;
        }
        case 2:
            system(CLEAR);
            createAccount();
            break;
        case 3:
            system(CLEAR);
            adminMenu();
            break;
        case 4:
            system(CLEAR);
            printf(CYAN "Exiting the program..." RESET);
            exit(0);
        default:
            printf(RED "INVALID INPUT.\n PLEASE TRY AGAIN\n" RESET);
        }
    }
}

void homeMenu(void) {
    int choice;

    while (1) {
        system(CLEAR);
        printf(CYAN "\n ACCOUNT HOME MENU \n" RESET);
        divider();
        printf(GREEN "\n1. Deposit Funds\n");
        printf("2. Withdraw Funds\n");
        printf("3. Transfer Funds\n" BLUE);
        printf("4. Display Account Details\n");
        printf("5. Transaction Details\n" RED);
        printf("6. Delete Account\n");
        printf("7. Logout\n" RESET);

        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            system(CLEAR);
            deposit();
            break;
        case 2:
            system(CLEAR);
            withdraw();
            break;
        case 3:
            system(CLEAR);
            transfer();
            break;
        case 4:
            system(CLEAR);
            display();
            break;
        case 5:
            system(CLEAR);
            transactionDetail();
            break;
        case 6:
            system(CLEAR);
            deleteAccount();
            if (current_user_index == -1) return;
            break;
        case 7:
            system(CLEAR);
            logout();
            return;
        default:
            printf(RED "INVALID INPUT.\n PLEASE TRY AGAIN\n" RESET);
        }
    }
}

void logout(void) {
    current_user_index = -1;
    printf(GREEN "Logged out successfully.\n" RESET);
    printf("\nPress any key...");
    getch();
}

void createAccount(void) {
    BankAccount newAccount;

    if (num >= MAX_ACCOUNTS) {
        printf(RED "Cannot create more accounts. The maximum limit has been reached.\n" RESET);
        return;
    }

    FILE *f1 = fopen("bank.txt", "a");
    FILE *f2 = fopen("account.txt", "a");

    system(CLEAR);
    printf(CYAN "\nACCOUNT CREATION\n" RESET);
    divider();
    printf("\nEnter Account Number: ");
    scanf("%d", &newAccount.account_number);
    printf("Enter Account Holder's Name: ");
    scanf(" ");
    fgets(newAccount.account_holder, 50, stdin);
    newAccount.account_holder[strcspn(newAccount.account_holder, "\n")] = '\0';
    printf("Enter Account Password: ");
    fgets(newAccount.password, 50, stdin);
    newAccount.password[strcspn(newAccount.password, "\n")] = '\0';
    printf("Enter 4-digit Pin For Transaction: ");
    scanf("%d", &newAccount.pin);
    printf("Enter Initial Balance: ");
    scanf("%f", &newAccount.balance);

    acc[num] = newAccount;
    num++;

    if (f2) {
        fprintf(f2, "Account Number: %d\nAccount Holder's Name: %s\nAccount Password: %s\n4-digit Pin: %d\nInitial balance: %.2f\n\n",
            newAccount.account_number, newAccount.account_holder,
            newAccount.password, newAccount.pin, newAccount.balance);
        fclose(f2);
    }
    if (f1) {
        fprintf(f1, "Initial balance for Account %d has been set to NPR %.2f\n\n",
            newAccount.account_number, newAccount.balance);
        fclose(f1);
    }

    printf(GREEN "\nAccount created successfully!\n" RESET);
    printf("\nPress any key...");
    getch();
}

void deposit(void) {
    system(CLEAR);
    printf(CYAN "\nDEPOSITING AMOUNT\n" RESET);
    divider();
    time_t tm;
    time(&tm);
    FILE *f = fopen("bank.txt", "a");
    float amt;

    printf("\nEnter the amount to deposit: ");
    scanf("%f", &amt);

    acc[current_user_index].balance += amt;
    printf(GREEN "Deposit Successful.\nUpdated balance: %.2f\n" RESET, acc[current_user_index].balance);

    if (f) {
        fprintf(f, "Your %d## has been Credited by NPR %.2f on %s",
            acc[current_user_index].account_number, amt, ctime(&tm));
        fprintf(f, "Remarks: Deposit. Download MoBank - http://bit.ly/MoBank4.\nADHIKARI BANK\n\n");
        fclose(f);
    }
    count++;

    printf("\nPress any key...");
    getch();
}

void withdraw(void) {
    float amt;
    int pin;
    time_t tm;
    time(&tm);
    FILE *f = fopen("bank.txt", "a");

    system(CLEAR);
    printf(CYAN "\nWITHDRAWING AMOUNT\n" RESET);
    divider();
    printf("\nEnter amount to withdraw: ");
    scanf("%f", &amt);
    printf("Enter your 4-digit pin: ");
    scanf("%d", &pin);

    if (acc[current_user_index].pin == pin) {
        if (amt > acc[current_user_index].balance) {
            printf(RED "Insufficient Balance.\n" RESET);
        } else {
            acc[current_user_index].balance -= amt;
            printf(GREEN "Withdrawal Successful!\nUpdated balance is: %.2f\n" RESET, acc[current_user_index].balance);
        }
    } else {
        printf(RED "INVALID PIN!\n" RESET);
    }

    if (f) {
        fprintf(f, "Your %d## has been Debited by NPR %.2f on %s",
            acc[current_user_index].account_number, amt, ctime(&tm));
        fprintf(f, "Remarks: Withdraw. Download MoBank - http://bit.ly/MoBank4.\nADHIKARI BANK\n\n");
        fclose(f);
    }
    count++;

    printf("\nPress any key...");
    getch();
}

void display(void) {
    system(CLEAR);
    printf(CYAN "\nACCOUNT DETAILS\n" RESET);
    divider();
    printf("\nAccount Number: %d\n", acc[current_user_index].account_number);
    printf("Account Holder: %s\n", acc[current_user_index].account_holder);
    printf("Account Balance: %.2f\n", acc[current_user_index].balance);

    printf("\nPress any key...");
    getch();
}

void transfer(void) {
    int acc_rec;
    float amt;
    int pin;
    int i;

    time_t tm;
    time(&tm);
    FILE *f = fopen("bank.txt", "a");

    system(CLEAR);
    printf(CYAN "\nTRANSFERRING MONEY\n" RESET);
    divider();
    printf("\nEnter the account number of receiver: ");
    scanf("%d", &acc_rec);
    printf("Enter amount to transfer: ");
    scanf("%f", &amt);
    printf("Enter your 4-digit pin: ");
    scanf("%d", &pin);

    if (acc[current_user_index].pin == pin) {
        if (amt > acc[current_user_index].balance) {
            printf(RED "Insufficient Balance.\n" RESET);
        } else {
            acc[current_user_index].balance -= amt;
            printf(GREEN "Transfer Successful to Account number %d.\nUpdated balance is: %.2f\n" RESET,
                acc_rec, acc[current_user_index].balance);

            if (f) {
                fprintf(f, "Your %d## has been Debited by NPR %.2f on %s",
                    acc[current_user_index].account_number, amt, ctime(&tm));
                fprintf(f, "Remarks: Transfer to acc %d##. Download MoBank - http://bit.ly/MoBank4.\nADHIKARI BANK\n\n",
                    acc_rec);
            }
            count++;

            for (i = 0; i < num; i++) {
                if (acc[i].account_number == acc_rec) {
                    acc[i].balance += amt;
                    if (f) {
                        fprintf(f, "Your %d## has been Credited by NPR %.2f on %s",
                            acc_rec, amt, ctime(&tm));
                        fprintf(f, "Remarks: Transfer from acc %d##. Download MoBank - http://bit.ly/MoBank4.\nADHIKARI BANK\n\n",
                            acc[current_user_index].account_number);
                    }
                    break;
                }
            }
        }
    } else {
        printf(RED "INVALID PIN!\n" RESET);
    }

    if (f) fclose(f);

    printf("\nPress any key...");
    getch();
}

void transactionDetail(void) {
    FILE *f = fopen("bank.txt", "r");
    char ch;

    system(CLEAR);
    printf(CYAN "\nTRANSACTION DETAILS\n" RESET);
    divider();
    printf("\nTOTAL TRANSACTIONS DONE: %d\n\n", count);

    if (!f || fscanf(f, "%c", &ch) == EOF) {
        printf(RED "No recent transactions\n" RESET);
    } else {
        putchar(ch);
        while ((ch = fgetc(f)) != EOF) putchar(ch);
    }
    if (f) fclose(f);

    printf("Press any key...");
    getch();
}

void adminMenu(void) {
    int pass;
    printf(CYAN "\nADMIN LOGIN\n" RESET);
    divider();
    printf("\nEnter admin password: ");
    scanf("%d", &pass);

    if (pass == 404) {
        int choice;
        while (1) {
            system(CLEAR);
            printf(CYAN "\n ADMIN MENU \n" RESET);
            divider();
            printf(GREEN "\n1. View All Transaction Details\n");
            printf("2. View Confidential Data\n" RED);
            printf("3. Logout\n" RESET);

            printf("Enter your choice: ");
            scanf("%d", &choice);

            switch (choice) {
            case 1:
                system(CLEAR);
                allTransactionDetails();
                break;
            case 2:
                system(CLEAR);
                confidential();
                break;
            case 3:
                return;
            default:
                printf(RED "INVALID INPUT.\n PLEASE TRY AGAIN\n" RESET);
            }
        }
    } else {
        printf(RED "ACCESS DENIED!\n" RESET);
        printf("\nPress any key...");
        getch();
    }
}

void allTransactionDetails(void) {
    FILE *f = fopen("bank.txt", "r");
    char ch;

    system(CLEAR);
    printf(CYAN "\nALL TRANSACTION DETAILS\n" RESET);
    divider();
    printf("\nTOTAL TRANSACTIONS DONE: %d\n\n", count);

    if (!f || fscanf(f, "%c", &ch) == EOF) {
        printf(RED "No recent transactions\n" RESET);
    } else {
        putchar(ch);
        while ((ch = fgetc(f)) != EOF) putchar(ch);
    }
    if (f) fclose(f);

    printf("Press any key...");
    getch();
}

void confidential(void) {
    FILE *f = fopen("account.txt", "r");
    char ch;
    int pass;

    system(CLEAR);
    printf(CYAN "\nCONFIDENTIAL DATA\n" RESET);
    divider();
    printf("\nEnter password for ACCESS: ");
    scanf("%d", &pass);

    if (pass == 404) {
        if (!f || fscanf(f, "%c", &ch) == EOF) {
            printf(RED "No data available\n" RESET);
        } else {
            putchar(ch);
            while ((ch = fgetc(f)) != EOF) putchar(ch);
        }
        if (f) fclose(f);
    } else {
        printf(RED "ACCESS DENIED!\n" RESET);
        if (f) fclose(f);
    }

    printf("Press any key...");
    getch();
}

void deleteAccount(void) {
    int acc_num, found = -1, i;
    printf(CYAN "Enter account number to delete: " RESET);
    scanf("%d", &acc_num);

    for (i = 0; i < num; i++) {
        if (acc[i].account_number == acc_num) {
            found = i;
            break;
        }
    }

    if (found != -1) {
        int is_current = (acc[current_user_index].account_number == acc_num);
        for (i = found; i < num - 1; i++) {
            acc[i] = acc[i + 1];
        }
        num--;
        printf(GREEN "Account %d deleted successfully!\n" RESET, acc_num);

        if (is_current) {
            current_user_index = -1;
            logout();
        }
    } else {
        printf(RED "Account not found. Please check the account number.\n" RESET);
    }

    printf("\nPress any key...");
    getch();
}

int main(void) {
    clearAccountData();
    mainMenu();
    return 0;
}
