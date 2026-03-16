#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

class Person
{
public:
    virtual void display() = 0;
    virtual ~Person() {}
};

class User : public Person
{
private:
    int id;
    string name;
    double balance;

public:
    User(int i, string n) : id(i), name(n), balance(0) {}

    int getId() { return id; }
    string getName() { return name; }
    double getBalance() { return balance; }

    void updateBalance(double amount) { balance += amount; }
    void setBalance(double b) { balance = b; }

    User operator+(User &u)
    {
        User temp(0, "temp");
        temp.setBalance(this->balance + u.balance);
        return temp;
    }

    void display() override
    {
        cout << id << ". " << name << " | Balance: Rs." << balance << "\n";
    }
};

class Group
{
private:
    vector<User> users;

    int findUser(int id)
    {
        for (int i = 0; i < (int)users.size(); i++)
            if (users[i].getId() == id)
                return i;
        return -1;
    }

public:
    void addUser(string name)
    {
        users.push_back(User(users.size() + 1, name));
        cout << "Friend added!\n";
    }

    void showUsers()
    {
        if (users.empty())
        {
            cout << "No users yet!\n";
            return;
        }
        cout << "\n--- Users ---\n";
        for (auto &u : users)
            u.display();
    }

    void addExpense(double amount, int paidById)
    {
        if (users.empty())
            throw runtime_error("No users in group!");
        if (amount <= 0)
            throw runtime_error("Amount must be positive!");
        if (findUser(paidById) == -1)
            throw runtime_error("User ID not found!");

        double split = amount / users.size();
        for (auto &u : users)
            u.updateBalance(u.getId() == paidById ? amount - split : -split);

        cout << "Expense split! Each person owes: Rs." << split << "\n";
    }

    void addExpense(double amount)
    {
        if (amount <= 0)
            throw runtime_error("Amount must be positive!");
        cout << "Expense of Rs." << amount << " recorded (not split).\n";
    }

    void addExpense(double amount, int paidById, string note)
    {
        cout << "[" << note << "] ";
        addExpense(amount, paidById);
    }

    void showSettlement()
    {
        if (users.empty())
        {
            cout << "No users!\n";
            return;
        }
        cout << "\n--- Final Settlement ---\n";

        vector<User> creditors, debtors;
        for (auto &u : users)
        {
            if (u.getBalance() > 0.01)
                creditors.push_back(u);
            else if (u.getBalance() < -0.01)
                debtors.push_back(u);
        }

        if (debtors.empty())
        {
            cout << "All settled! No payments needed.\n";
            return;
        }

        int i = 0, j = 0;
        while (i < (int)debtors.size() && j < (int)creditors.size())
        {
            double debt = -debtors[i].getBalance();
            double credit = creditors[j].getBalance();
            double settle = min(debt, credit);

            cout << debtors[i].getName() << " pays "
                 << creditors[j].getName() << " Rs." << settle << "\n";

            debtors[i].setBalance(debtors[i].getBalance() + settle);
            creditors[j].setBalance(creditors[j].getBalance() - settle);

            if (debtors[i].getBalance() > -0.01)
                i++;
            if (creditors[j].getBalance() < 0.01)
                j++;
        }
    }
};

int main()
{
    Group group;
    int choice, paidBy, subChoice;
    string name, note;
    double amount;

    do
    {
        cout << "\n===== Smart Expense Splitter =====\n";
        cout << "1. Add Friend\n2. Add Expense\n3. Show Users\n4. Show Settlement\n5. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        if (cin.fail())
        {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid! Please enter a number (1-5).\n";
            continue;
        }

        switch (choice)
        {
        case 1:
            cout << "Enter friend name: ";
            cin >> name;
            group.addUser(name);
            break;

        case 2:
            cout << "Enter amount: ";
            cin >> amount;
            cout << "1. Split equally\n2. Just record (no split)\n3. Split with note\nChoice: ";
            cin >> subChoice;
            try
            {
                if (subChoice == 1)
                {
                    cout << "Paid by (User ID): ";
                    cin >> paidBy;
                    group.addExpense(amount, paidBy);
                }
                else if (subChoice == 2)
                {
                    group.addExpense(amount);
                }
                else if (subChoice == 3)
                {
                    cout << "Enter note: ";
                    cin >> note;
                    cout << "Paid by (User ID): ";
                    cin >> paidBy;
                    group.addExpense(amount, paidBy, note);
                }
                else
                {
                    cout << "Invalid sub-choice!\n";
                }
            }
            catch (runtime_error &e)
            {
                cout << "Error: " << e.what() << "\n";
            }
            break;

        case 3:
            group.showUsers();
            break;
        case 4:
            group.showSettlement();
            break;
        case 5:
            cout << "Bye!\n";
            break;
        default:
            cout << "Invalid choice! Try 1-5.\n";
        }

    } while (choice != 5);

    return 0;
}
