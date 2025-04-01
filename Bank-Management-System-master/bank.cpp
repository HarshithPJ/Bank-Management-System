#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <conio.h>
#include <cstdlib>
#include <iomanip>

using namespace std;

// Function Declarations
void fordelay(int);
void close();
void menu_e();
void employee();
void customer();
void menu_c();

class Bank {
public:
    string account_number;
    string name;
    string dob;
    string age;
    string address;
    string phone;
    string depo;
    string acc_type;
    string password;
    string date;
    string doc;
    string interest;

    void read_data();
    void acc_write();
    void modify_account();
    void search_rec();
    void deposit_withdraw();
    void display_all();
    void transfer();
};

// Function to calculate interest
float calculate_interest(float time, float amount, int rate) {
    return (rate * time * amount) / 100.0;
}

// Function to read account data
void Bank::read_data() {
    cout << "\nEnter Account Number: ";
    cin >> account_number;

    // Check if account already exists
    ifstream is("Account_info.csv");
    string acc_no, pass;
    while (getline(is, acc_no, ',')) {
        getline(is, pass, '\n');
        if (acc_no == account_number) {
            cout << "\nAccount No. Matches with Existing Account!";
            cout << "\n1. Try Again!\n2. Login To Account\n:";
            int x;
            cin >> x;
            if (x == 1) {
                read_data(); // Retry
                return;
            } else {
                customer(); // Login
                return;
            }
        }
    }
    is.close();

    // Input new account details
    cout << "\nEnter Name: ";
    cin.ignore();
    getline(cin, name);
    cout << "\nEnter Date of Birth (mm/dd/yyyy): ";
    cin >> dob;
    cout << "\nEnter Age: ";
    cin >> age;
    cout << "\nEnter Address: ";
    cin.ignore();
    getline(cin, address);
    cout << "\nEnter Phone Number: ";
    cin >> phone;
    cout << "\nYou want to deposit amount:\n\t1.Yes\n\t2.No\n:";
    int x;
    cin >> x;
    if (x == 1) {
        cout << "\nEnter the amount to deposit: Rs ";
        cin >> depo;
    } else {
        depo = "0";
    }
    cout << "\nType of account:\n\t#Saving\n\t#Current\n\n\tEnter your choice: ";
    cin >> acc_type;

    // Record creation date and time
    time_t now = time(0);
    date = ctime(&now);
    struct tm *ltm = localtime(&now);
    doc = to_string(ltm->tm_mday);

    cout << "\nEnter Password For Your Account: ";
    cin >> password;
    cout << "\nAccount created successfully!";
}

// Function to write account data to CSV
void Bank::acc_write() {
    read_data();
    ofstream MyFile1("Bank_Record.csv", ios::app);
    MyFile1 << account_number << "," << name << "," << dob << ","
            << age << "," << address << "," << phone << "," << depo
            << "," << acc_type << "," << date << "," << doc << "," << interest << "\n";
    MyFile1.close();

    ofstream MyFile2("Account_info.csv", ios::app);
    MyFile2 << account_number << "," << password << "\n";
    MyFile2.close();
}

// Function to modify account details
void Bank::modify_account() {
    string acc_no;
    cout << "Enter the Account Number to modify: ";
    cin >> acc_no;

    ifstream is("Bank_Record.csv");
    ofstream temp("temp.csv");
    bool found = false;
    string line;

    while (getline(is, line)) {
        size_t pos = line.find(acc_no);
        if (pos != string::npos) {
            found = true;
            cout << "\nAccount Found! Enter new details:\n";
            read_data();
            temp << account_number << "," << name << "," << dob << ","
                 << age << "," << address << "," << phone << "," << depo
                 << "," << acc_type << "," << date << "," << doc << "," << interest << "\n";
        } else {
            temp << line << "\n";
        }
    }
    is.close();
    temp.close();

    if (found) {
        remove("Bank_Record.csv");
        rename("temp.csv", "Bank_Record.csv");
        cout << "\nAccount updated successfully!";
    } else {
        cout << "\nAccount not found!";
        remove("temp.csv");
    }
}

// Function to search for an account
void Bank::search_rec() {
    string acc_no;
    cout << "Enter the Account Number to search: ";
    cin >> acc_no;

    ifstream is("Bank_Record.csv");
    string line;
    bool found = false;

    while (getline(is, line)) {
        size_t pos = line.find(acc_no);
        if (pos != string::npos) {
            found = true;
            cout << "\nAccount Details:\n" << line << endl;
            break;
        }
    }
    is.close();

    if (!found) {
        cout << "\nAccount not found!";
    }
}

// Function to deposit or withdraw money
void Bank::deposit_withdraw() {
    string acc_no;
    int choice, amount;
    cout << "Enter the Account Number: ";
    cin >> acc_no;

    ifstream is("Bank_Record.csv");
    ofstream temp("temp.csv");
    string line;
    bool found = false;

    while (getline(is, line)) {
        size_t pos = line.find(acc_no);
        if (pos != string::npos) {
            found = true;
            cout << "\nAccount Found!\n";
            cout << "1. Deposit\n2. Withdraw\nEnter your choice: ";
            cin >> choice;
            cout << "Enter the amount: ";
            cin >> amount;

            // Extract current balance
            size_t balance_pos = line.find_last_of(',');
            string balance_str = line.substr(balance_pos + 1);
            int balance = stoi(balance_str);

            if (choice == 1) {
                balance += amount;
            } else if (choice == 2) {
                if (balance >= amount) {
                    balance -= amount;
                } else {
                    cout << "\nInsufficient balance!";
                    return;
                }
            } else {
                cout << "\nInvalid choice!";
                return;
            }

            // Update the line with new balance
            line.replace(balance_pos + 1, string::npos, to_string(balance));
            temp << line << "\n";
            cout << "\nTransaction successful!";
        } else {
            temp << line << "\n";
        }
    }
    is.close();
    temp.close();

    if (found) {
        remove("Bank_Record.csv");
        rename("temp.csv", "Bank_Record.csv");
    } else {
        cout << "\nAccount not found!";
        remove("temp.csv");
    }
}

// Function to display all accounts
void Bank::display_all() {
    ifstream is("Bank_Record.csv");
    string line;
    cout << "\nAll Accounts:\n";
    while (getline(is, line)) {
        cout << line << endl;
    }
    is.close();
}

// Function to transfer money between accounts
void Bank::transfer() {
    string from_acc, to_acc;
    int amount;
    cout << "Enter your Account Number: ";
    cin >> from_acc;
    cout << "Enter the Account Number to transfer to: ";
    cin >> to_acc;
    cout << "Enter the amount to transfer: ";
    cin >> amount;

    // Implement transfer logic here (similar to deposit/withdraw)
    // This is left as an exercise for the reader
    cout << "\nTransfer functionality not implemented yet!";
}

// Function to close the program
void close() {
    cout << "\nThank you for using the Bank Management System. Goodbye!\n";
    exit(0);
}

// Function to display the employee menu
void menu_e() {
    Bank b;
    int choice;
    cout << "\nEmployee Menu:\n";
    cout << "1. Create Account\n2. Modify Account\n3. Display All Accounts\n4. Exit\nEnter your choice: ";
    cin >> choice;

    switch (choice) {
        case 1: b.acc_write(); break;
        case 2: b.modify_account(); break;
        case 3: b.display_all(); break;
        case 4: close(); break;
        default: cout << "\nInvalid choice!";
    }
}

// Function to display the customer menu
void menu_c() {
    Bank b;
    int choice;
    cout << "\nCustomer Menu:\n";
    cout << "1. Deposit/Withdraw\n2. Check Balance\n3. Transfer Money\n4. Exit\nEnter your choice: ";
    cin >> choice;

    switch (choice) {
        case 1: b.deposit_withdraw(); break;
        case 2: b.search_rec(); break;
        case 3: b.transfer(); break;
        case 4: close(); break;
        default: cout << "\nInvalid choice!";
    }
}

// Function to handle employee login
void employee() {
    string password;
    cout << "\nEnter Employee Password: ";
    cin >> password;

    if (password == "1234") {
        menu_e();
    } else {
        cout << "\nIncorrect Password!";
    }
}

// Function to handle customer login
void customer() {
    string password;
    cout << "\nEnter Customer Password: ";
    cin >> password;

    if (password == "1234") {
        menu_c();
    } else {
        cout << "\nIncorrect Password!";
    }
}

// Main function
int main() {
    int ch;
    cout << "\nWelcome to the Bank Management System!\n";
    cout << "1. Employee Login\n2. Customer Login\n3. Exit\nEnter your choice: ";
    cin >> ch;

    switch (ch) {
        case 1: employee(); break;
        case 2: customer(); break;
        case 3: close(); break;
        default: cout << "\nInvalid choice!";
    }

    return 0;
}