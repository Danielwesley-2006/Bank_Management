#include <iostream>
#include <vector>
#include <stdexcept>
#include <cmath>
#include<algorithm>
#include <iomanip>
#include <fstream> 
#include<unordered_map>

using namespace std;

unordered_map<int, string> accountPasswords; 

class BankAccount {
public:
    static constexpr const char* IFSC_CODE ="LOKI069069";
    string accountHolderName;
    int accountNumber;
    double balance;
    string phoneNumber; 
    string aadhaarNumber;


    BankAccount(string name, int number, double initialBalance, string phone, string aadhaar) 
        : accountHolderName(name), accountNumber(number), balance(initialBalance),phoneNumber(phone), aadhaarNumber(aadhaar) {}

    virtual void deposit(double amount) {
        balance += amount;
        cout << "Deposited $" << amount << " into account number " << accountNumber << endl;
    }

    virtual void withdraw(double amount) {
        if (amount > balance) {
            throw runtime_error("Insufficient balance!");
        }
        balance -= amount;
        cout << "Withdrew $" << amount << " from account number " << accountNumber << endl;
    }

    virtual void displayBalance() const {
        cout << "\n----------------------------------\n";
        cout << "    Account Details\n";
        cout << "----------------------------------\n";
        cout << left << setw(20) << "Account Holder:" << accountHolderName << endl;
        cout << left << setw(20) << "Account Number:" << accountNumber << endl;
        cout << left << setw(20) << "IFSC Code:" << IFSC_CODE << endl;
        cout << left << setw(20) << "Phone Number:" << phoneNumber << endl;
        cout << left << setw(20) << "Aadhaar Number:" << aadhaarNumber << endl;
        cout << left << setw(20) << "Current Balance: $" << balance << endl;
        cout << "----------------------------------\n"; 
    }

    virtual ~BankAccount() {}
};

bool isValidPhoneNumber(const string& phone) {
    return phone.length() == 10 && (phone[0] == '7' || phone[0] == '8' || phone[0] == '9');
}

bool isValidAadhaarNumber(const string& aadhaar) {
    return aadhaar.length() == 12 && all_of(aadhaar.begin(), aadhaar.end(), ::isdigit);
}

class SavingsAccount : public BankAccount {
public:
    double interestRate;

    SavingsAccount(string name, int number, double initialBalance,string phoneNumber,string aadhaarNumber)
        : BankAccount(name, number, initialBalance,phoneNumber,aadhaarNumber), interestRate(0.02) {}

    void applyInterest() {
        balance += balance * interestRate;
        cout << "Applied interest, new balance: $" << balance << endl;
    }
};

class CheckingAccount : public BankAccount {
public:
    double transactionFee;

    CheckingAccount(string name, int number, double initialBalance,string phoneNumber, string aadhaarNumber)
        : BankAccount(name, number, initialBalance,phoneNumber,aadhaarNumber), transactionFee(15.0) {}

    void withdraw(double amount) override {
        double totalAmount = amount + transactionFee;
        BankAccount::withdraw(totalAmount);
        cout << "Charged a transaction fee of $" << transactionFee << ", total withdrawal: $" << totalAmount << endl;
    }
};

class BusinessAccount : public BankAccount {
public:
    double loanEligibilityAmount;

    BusinessAccount(string name, int number, double initialBalance,string phoneNumber, string aadhaarNumber, double loanEligibilityAmount)
        : BankAccount(name, number, initialBalance,phoneNumber,aadhaarNumber), loanEligibilityAmount(loanEligibilityAmount) {}

    void approveLoan(double loanAmount) {
        if (loanEligibilityAmount < loanAmount) {
            throw runtime_error("Loan not approved. Insufficient balance for eligibility.");
        }
        cout << "Loan of $" << fixed << setprecision(0) << loanAmount << " approved for account number " << accountNumber << endl;
    }
};

class Loan {
public:
    double loanAmount;
    double interestRate;
    int loanTerm;
    string accountHolderName;
    int accountNumber;
    
    Loan(double amount, double rate, int term, string name, int number)
        : loanAmount(amount), interestRate(rate), loanTerm(term),
          accountHolderName(name), accountNumber(number) {}

    double calculateMonthlyPayment() {
        double monthlyRate = interestRate / 12;
        return (loanAmount * monthlyRate) / (1 - pow(1 + monthlyRate, -loanTerm));
    }
};

void applyLoan(vector<BankAccount*>& accounts) {
    int accNum;
    cout << "Enter Account Number for Loan Application: ";
    cin >> accNum;

    for (auto& acc : accounts) {
        if (acc->accountNumber == accNum) {
            double loanAmount, interestRate;
            int loanTerm;
            cout << "Enter Loan Amount: ";
            cin >> loanAmount;
            cout << "Enter Interest Rate (e.g., 0.05 for 5%): ";
            cin >> interestRate;
            cout << "Enter Loan Term (in months): ";
            cin >> loanTerm;

            Loan loan(loanAmount, interestRate, loanTerm, acc->accountHolderName, acc->accountNumber);
            ofstream file("loans.txt", ios::app);
            if (file) {
                file << loan.accountHolderName << " " << loan.accountNumber << " "
                     << loan.loanAmount << " " << loan.interestRate << " " << loan.loanTerm << endl;
                cout << "Loan application saved for " << loan.accountHolderName
                     << " with monthly payment: $" << loan.calculateMonthlyPayment() << endl;
            } else {
                cerr << "Error opening loan file for writing." << endl;
            }
            file.close();
            return;
        }
    }
    cout << "Account not found." << endl;
}


class Investment {
public:
    double investmentAmount;
    double rateOfReturn;
    string accountHolderName;
    int accountNumber;

    Investment(double amount, double rate, string name, int number)
        : investmentAmount(amount), rateOfReturn(rate),
          accountHolderName(name), accountNumber(number) {}

    double calculateReturn(int years) {
        return investmentAmount * pow(1 + rateOfReturn, years);
    }
};

void manageInvestment(vector<BankAccount*>& accounts) {
    int accNum;
    cout << "Enter Account Number for Investment Management: ";
    cin >> accNum;

    for (auto& acc : accounts) {
        if (acc->accountNumber == accNum) {
            double amount, rate;
            int years;
            cout << "Enter Investment Amount: ";
            cin >> amount;
            cout << "Enter Rate of Return (e.g., 0.07 for 7%): ";
            cin >> rate;
            cout << "Enter Investment Duration (years): ";
            cin >> years;

            Investment investment(amount, rate, acc->accountHolderName, acc->accountNumber);
            ofstream file("investments.txt", ios::app);
            if (file) {
                file << investment.accountHolderName << " " << investment.accountNumber << " "
                     << investment.investmentAmount << " " << investment.rateOfReturn << " " << years << endl;
                cout << "Investment details saved for " << investment.accountHolderName
                     << " with estimated return after " << years << " years: $"
                     << fixed << setprecision(2) << investment.calculateReturn(years) << endl;
            } else {
                cerr << "Error opening investment file for writing." << endl;
            }
            file.close();
            return;
        }
    }
    cout << "Account not found." << endl;
}

// File handling functions for accounts
void saveAccounts(const vector<BankAccount*>& accounts) {
    ofstream file("accounts.txt");
    if (!file) {
        cerr << "Error opening file for writing." << endl;
        return;
    }
    for (const auto& acc : accounts) {
        if (dynamic_cast<SavingsAccount*>(acc)) {
            file << "Savings " << acc->accountHolderName << " " << acc->accountNumber << " " 
                 << acc->balance << " " << acc->phoneNumber << " " << acc->aadhaarNumber << endl;
        } else if (dynamic_cast<CheckingAccount*>(acc)) {
            file << "Checking " << acc->accountHolderName << " " << acc->accountNumber << " " 
                 << acc->balance << " " << acc->phoneNumber << " " << acc->aadhaarNumber << endl;
        } else if (dynamic_cast<BusinessAccount*>(acc)) {
            BusinessAccount* businessAcc = dynamic_cast<BusinessAccount*>(acc);
            file << "Business " << businessAcc->accountHolderName << " " << businessAcc->accountNumber 
                 << " " << businessAcc->balance << " " << businessAcc->loanEligibilityAmount << " " 
                 << businessAcc->phoneNumber << " " << businessAcc->aadhaarNumber << endl;
        }
    }
    file.close();
}

void saveLastAccountNumber(int accountNumber) {
    ofstream file("last_account_number.txt");
    if (!file) {
        cerr << "Error opening last account number file for writing." << endl;
        return;
    }
    file << accountNumber;
    file.close();
}

void savePassword(int accountNumber, const string& password, const string& name) {
    ofstream file("passwords.txt", ios::app);
    if (file) {
        file << accountNumber << " " << password << " " << name << endl;
        cout << "Password saved for account " << accountNumber << endl;
    } else {
        cerr << "Error saving password.\n";
    }
}

void saveLoan(const Loan& loan) {
    ofstream file("loans.txt", ios::app);
    if (!file) {
        cerr << "Error opening file for writing." << endl;
        return;
    }
    file << loan.loanAmount << " " << loan.interestRate << " " << loan.loanTerm << endl;
    file.close();
}

int loadLastAccountNumber() {
    ifstream file("last_account_number.txt");
    int accountNumber = 100000000; // Default starting account number
    if (file) {
        file >> accountNumber;
        file.close();
    }
    return accountNumber;
}


void loadAccounts(vector<BankAccount*>& accounts) {
    ifstream file("accounts.txt");
    if (!file) {
        cerr << "No saved accounts to load." << endl;
        return;
    }

    string accountType, name, phone, aadhaar;
    int number;
    double balance, loanEligibility;

    while (file >> accountType >> name >> number >> balance >> phone >> aadhaar) {
        if (accountType == "Savings") {
            accounts.push_back(new SavingsAccount(name, number, balance, phone, aadhaar));
        } else if (accountType == "Checking") {
            accounts.push_back(new CheckingAccount(name, number, balance, phone, aadhaar));
        } else if (accountType == "Business") {
            file >> loanEligibility;
            accounts.push_back(new BusinessAccount(name, number, balance, phone, aadhaar, loanEligibility));
        }
    }
    file.close();
}

bool loadPasswords() {
    ifstream file("passwords.txt");
    if (!file) {
        cerr << "No saved passwords to load.\n";
        return false;
    }
    int accNum;
    string password, name;
    while (file >> accNum >> password >> name) {
        accountPasswords[accNum] = password;
    }
    file.close();
    return true;
}

bool verifyPassword(int accountNumber) {
    string enteredPassword;
    cout << "Enter password for account " << accountNumber << ": ";
    cin >> enteredPassword;
    return accountPasswords[accountNumber] == enteredPassword;
}

void createAccount(vector<BankAccount*>& accounts, int& accountNumber) {
    string name,password;
    double initialDeposit;
    int type;
    string phone, aadhaar; 

    cout << "Enter Account Holder Name: ";
    cin.ignore(); 
    getline(cin, name);

    do {
        cout << "Enter 10-digit Phone Number (starting with 7, 8, or 9): ";
        cin >> phone;
        if (!isValidPhoneNumber(phone)) {
            cout << "Invalid phone number! Please try again.\n";
        }
    } while (!isValidPhoneNumber(phone));

    // Validate Aadhaar number
    do {
        cout << "Enter 12-digit Aadhaar Number: ";
        cin >> aadhaar;
        if (!isValidAadhaarNumber(aadhaar)) {
            cout << "Invalid Aadhaar number! Please try again.\n";
        }
    } while (!isValidAadhaarNumber(aadhaar));


    cout << "\n*** SELECT ACCOUNT TYPE ***\n1. Savings\n2. Checking\n3. Business\nEnter choice: ";
    cin >> type;

    cout << "Set a password for the account: ";
    cin >> password;

    cout << "Enter Initial Deposit: ";
    cin >> initialDeposit;

    if (type == 1) {
        accounts.push_back(new SavingsAccount(name, accountNumber, initialDeposit,phone,aadhaar));
        cout << "Savings Account created with Account Number: " << accountNumber << endl;
    } else if (type == 2) {
        accounts.push_back(new CheckingAccount(name, accountNumber, initialDeposit,phone,aadhaar));
        cout << "Checking Account created with Account Number: " << accountNumber << endl;
    } else if (type == 3) {
        double loanEligibility;
        cout << "Enter Loan Eligibility Amount: ";
        cin >> loanEligibility;
        accounts.push_back(new BusinessAccount(name, accountNumber, initialDeposit,phone,aadhaar, loanEligibility));
        cout << "Business Account created with Account Number: " << accountNumber << endl;
    } else {
        cout << "Invalid Account Type Selected.\n";
        return;
    }

    savePassword(accountNumber, password, name);
    accountPasswords[accountNumber] = password;

    accountNumber++;
    saveLastAccountNumber(accountNumber);
    saveAccounts(accounts);
}

void userInterface(vector<BankAccount*>& accounts, int& accountNumber) {
    int choice;
    while (true) {
        cout << "\n--- USER INTERFACE ---\n";
        cout << "1. Create Account\n";
        cout << "2. Deposit Money\n";
        cout << "3. Withdraw Money\n";
        cout << "4. Display Account Details\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                createAccount(accounts, accountNumber);
                break;
            case 2: {
                int accNum;
                double amount;
                cout << "Enter Account Number: ";
                cin >> accNum;

                if (!verifyPassword(accNum)) {
                    cout << "Incorrect password! Access denied.\n";
                    return;
                }
                cout << "Enter Deposit Amount: ";
                cin >> amount;

                bool found = false;
                for (auto& acc : accounts) {
                    if (acc->accountNumber == accNum) {
                        acc->deposit(amount);
                        saveAccounts(accounts);
                        found = true;
                        break;
                    }
                }
                if (!found) cout << "Account not found.\n";
                break;
            }
            case 3: {
                int accNum;
                double amount;
                cout << "Enter Account Number: ";
                cin >> accNum;

                if (!verifyPassword(accNum)) {
                    cout << "Incorrect password! Access denied.\n";
                    return;
                }

                cout << "Enter Withdrawal Amount: ";
                cin >> amount;

                bool found = false;
                for (auto& acc : accounts) {
                    if (acc->accountNumber == accNum) {
                        acc->withdraw(amount);
                        saveAccounts(accounts);
                        found = true;
                        break;
                    }
                }
                if (!found) cout << "Account not found.\n";
                break;
            }
            case 4: {
                int accNum;
                cout << "Enter Account Number: ";
                cin >> accNum;

                if (!verifyPassword(accNum)) {
                    cout << "Incorrect password! Access denied.\n";
                    return;
                }


                bool found = false;
                for (auto& acc : accounts) {
                    if (acc->accountNumber == accNum) {
                        acc->displayBalance();
                        found = true;
                        break;
                    }
                }
                if (!found) cout << "Account not found.\n";
                break;
            }
            case 5:
                cout << "OK !!\nExiting User Interface.\n";
                return;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    }
}

void bankerInterface(vector<BankAccount*>& accounts, int& accountNumber) {
    int choice;
    while (true) {
        cout << "\n--- BANKER INTERFACE ---\n";
        cout << "1. Create Account\n";
        cout << "2. Deposit Money\n";
        cout << "3. Withdraw Money\n";
        cout << "4. Apply for Loan\n";
        cout << "5. Manage Investment\n";
        cout << "6. Display Account Details\n";
        cout << "7. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                createAccount(accounts, accountNumber);
                break;
            case 2: {
            int accNum;
            double amount;
            cout << "Enter Account Number: ";
            cin >> accNum;
            cout << "Enter Deposit Amount: ";
            cin >> amount;

            bool found = false;
            for (auto &acc : accounts) {
                if (acc->accountNumber == accNum) {
                    acc->deposit(amount);
                    saveAccounts(accounts);
                    found = true;
                    break;
                }
            }
            if (!found) cout << "Account not found.\n";
            break;
            }
            case 3: {
            int accNum;
            double amount;
            cout << "Enter Account Number: ";
            cin >> accNum;
            cout << "Enter Withdrawal Amount: ";
            cin >> amount;

            bool found = false;
            for (auto &acc : accounts) {
                if (acc->accountNumber == accNum) {
                    acc->withdraw(amount);
                    saveAccounts(accounts);
                    found = true;
                    break;
                }
            }
            if (!found) cout << "Account not found.\n";
            break;
            }
            case 4:{
                applyLoan(accounts); break;
            }
            case 5:{
                manageInvestment(accounts); break;
            }
            case 6: {
            int accNum;
            cout << "Enter Account Number: ";
            cin >> accNum;

            bool found = false;
            for (auto &acc : accounts) {
                if (acc->accountNumber == accNum) {
                    acc->displayBalance();
                    found = true;
                    break;
                }
            }
            if (!found) cout << "Account not found.\n";
            break; 
            }
            case 7:{
                cout << "OK !!\nExiting the interface.\n";
                return;
            }
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    }
}

// Main function
int main() {
    vector<BankAccount*> accounts;
    int choice;
    int accountNumber = 100000000;

    // Load existing accounts from file
    loadAccounts(accounts);
    accountNumber = loadLastAccountNumber();
    loadPasswords();

    int interfaceChoice;
    cout << "Select Interface:\n1. Banker Interface\n2. User Interface\nEnter choice: ";
    cin >> interfaceChoice;

    if (interfaceChoice == 1) {
        bankerInterface(accounts, accountNumber);
    } else if (interfaceChoice == 2) {
        userInterface(accounts, accountNumber);
    } else {
        cout << "Invalid Interface Choice.\n";
    }

    // Clean up memory
    for (auto& acc : accounts) {
        delete acc;
    }
    return 0; 
}

