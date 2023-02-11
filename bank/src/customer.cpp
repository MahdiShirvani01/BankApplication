#include "../header/customer.hpp"
#include <ctime>
#include <stdexcept>
#include <string>

int customer::customer_count = 0;

// customer constructor
customer::customer()
{
    // store customers
    customer_count++;
}

// setters function
void customer::set_name(string a_name)
{
    username = a_name;
}

void customer::set_cadr_num(int a_card)
{
    card_number = a_card;
}

bool customer::set_ip(string a_ip)
{
    try
    {
        for (auto item : ip) // check for unique IP
        {
            if (item == a_ip)
            {
                throw invalid_argument("IP is available!!!");
            }
        }
        ip.push_back(a_ip);
        return true;
    }
    catch (invalid_argument &w)
    {
        std::cerr << w.what() << endl;
        return false;
    }
}

void customer::set_stock()
{
    stock = 0;
}

void customer::set_borrow(int amount)
{
    borrow = amount;
}

void customer::set_date()
{

    time_t now = time(0);
    tm *ltm = localtime(&now);
    // set date and time in 1st column
    array[0].year = ltm->tm_year + 1900;
    array[0].month = 1 + ltm->tm_mon;
    array[0].day = ltm->tm_mday;
    // set expiration date in 2nd column
    array[1].year = ltm->tm_year + 1900 + 2;
    array[1].month = 1 + ltm->tm_mon;
    array[1].day = ltm->tm_mday;
}
//--------------------------------

// getters function
string customer::get_username()
{
    return username;
}

int customer::get_card_num()
{
    return card_number;
}

int customer::get_stock()
{
    return stock;
}

int customer::get_borrow()
{
    return borrow;
}

int customer::get_count()
{
    return customer_count;
}

void customer::get_ip()
{
    for (auto A : this->ip)
    {
        cout << "Active IP: " << A << endl;
    }
}

string customer::get_opening_date()
{
    string temp = "";
    temp = to_string(array[0].month) + '/' + to_string(array[0].day) + '/' + to_string(array[0].year);
    return temp;
}

string customer::get_expiration_date()
{
    string temp = "";
    temp = to_string(array[1].month) + '/' + to_string(array[1].day) + '/' + to_string(array[1].year);
    return temp;
}
// transaction functions
bool customer::transaction(int amount, int status, vector<customer> &v_customer, vector<banktransaction>&AT)
{
    // if status == 0 ---> deposit
    // if status == 1 ---> whitdraw
    // if status == 2 ---> transfer
    time_t now = time(0);
    tm *ltm = localtime(&now);
    try
    {
        // check curent year with expiration year
        if ((ltm->tm_year + 1900) > array[1].year)
        {
            throw out_of_range("you can't do transaction!you must renewal account");
        }
        // check curent month with expiration month if(curent year == expiration year)
        if (((ltm->tm_year + 1900) == array[1].year) && ltm->tm_mon + 1 > array[1].month)
        {
            throw out_of_range("you can't do transaction!you must renewal account");
        }
        // check curent day with expiration day if(curent year == expiration year &&  curent month == expiration month)
        if (((ltm->tm_year + 1900) == array[1].year) && (ltm->tm_mon + 1 == array[1].month) && ltm->tm_mday > array[1].day)
        {
            throw out_of_range("you can't do transaction!you must renewal account");
        }
        int balance_of_bank{0};
        for (auto item : v_customer)
        {
            if (item.get_username() == this->get_username())
            {
            }
            else
            {
                balance_of_bank = item.get_stock() + item.get_borrow() + balance_of_bank;
            }
        }
        if (status == DEPOSIT)
        {
            if (this->get_borrow() < 0)
            {
                int sum = this->get_borrow() + amount;
                if (sum < 0)
                {
                    this->set_borrow(sum);
                }
                if (sum == 0)
                {
                    banktransaction temp{sum};
                    temp.set_loan_status(NOBORROW);
                    temp.set_source(this->get_username());
                    AT.push_back(temp);
                    this->save_transactions(AT);
                    this->set_borrow(0);
                }
                if (sum > 0)
                {
                    this->set_borrow(0);
                    banktransaction temp{sum};
                    temp.set_loan_status(NOBORROW);
                    temp.set_source(this->get_username());
                    AT.push_back(temp);
                    this->save_transactions(AT);
                    stock = stock + sum;
                    cout << "Deposit " << sum << " Rials" << endl;
                }
            }
            else
            {
                stock = stock + amount;
                cout << "Deposit " << amount << " Rials" << endl;
            }
        }
        if (status == WITHDRAW)
        {
            if (this->get_borrow() == 0) // indebteding user?
            {
                if (stock >= (-amount))
                {
                    if (balance_of_bank >= (-amount)) // is balance of bank enough for giving money?
                    {
                        stock = stock + amount;
                        cout << "withdraw " << -amount << " Rials" << endl;
                        return true;
                    }
                    else
                    {
                        throw out_of_range("Sorry the bank is bankrupt!");
                    }
                }
                else
                {
                    throw out_of_range("threr is not enough stock for withdraw!!");
                }
            }
            else
            {
                throw out_of_range("you can't withdrae! (you must pay last loan!)");
            }
        }
    }
    catch (const out_of_range &w)
    {
        std::cerr << w.what() << '\n';
        return false;
    }
    return true;
}

// checking IP
bool customer::checkIP(string ip_a)
{
    for (auto item : ip)
    {
        if (item == ip_a)
        {
            return true;
        }
    }
    return false;
}

// store transactions
void customer::save_transactions(vector<banktransaction> &AT) // AT = all transaction
{
    // push element number of AT in the customer::transactions(vector)
    transactions.push_back(AT.size() - 1);
}

// getting transactions size
int customer::get_size_transactions()
{
    return transactions.size(); // return size of vector
}

// take_profits method
bool customer::take_profites(vector<banktransaction> &AT, vector<customer> &v_customer)
{
    time_t now = time(0);
    tm *ltm = localtime(&now);
    int sum{0};
    int DAY{0};
    int MONTH{0};
    int YEAR{0};
    try
    {
        if (this->get_borrow() < 0) // if user has a debit to the bank
        {
            throw invalid_argument("This user can not take profit! (must pay loan)");
        }
        else if (this->get_borrow() == 0)
        {
            for (int j = 0; j < transactions.size(); j++) // checking last pay loan date
            {
                if (AT[transactions[j]].get_loan_status() == NOBORROW)
                {
                    DAY = (ltm->tm_mday) - AT[transactions[j]].get_day(); // calculate daysfrom current day
                    if (DAY < 0)                                          // set correct number --->   day > 0 
                    {
                        DAY += 30;
                    }

                    YEAR = (ltm->tm_year + 1900) - AT[transactions[j]].get_year(); // calculate tear from current year
                    if (YEAR == 0)
                    {
                        MONTH = (1 + ltm->tm_mon) - AT[transactions[j]].get_month(); // calculate month from current month
                        if (MONTH < 0)
                        {
                            MONTH += 12;
                        }
                        if ((MONTH * DAY) < 30) // checking less than 30 days difference
                        {
                            throw out_of_range("you can't add  profit! (30 days must pass from the payment of the last loan)");
                        }
                    }
                }
            }
        }
        DAY = 0;
        MONTH = 0;
        YEAR = 0;
        for (int j = 0; j < this->transactions.size(); j++) // checking last profit
        {
            DAY = (ltm->tm_mday) - AT[transactions[j]].get_day(); // calculate day from current day
            if (DAY < 0)                                          // set correct number --->   day > 0
            {
                DAY += 30;
            }
            if (AT[transactions[j]].get_status() == PROFIT)
            {
                YEAR = (ltm->tm_year + 1900) - AT[transactions[j]].get_year(); //calculate tear from current year
                if (YEAR == 0)
                {
                    MONTH = (1 + ltm->tm_mon) - AT[transactions[j]].get_month(); //calculate month from current month
                    if (MONTH < 0)
                    {
                        MONTH += 12;
                    }
                    if ((MONTH * DAY) < 30) // checking less than 30 days difference
                    {
                        throw out_of_range("you can't add another profit!");
                    }
                }
            }
            if (DAY <= 7)
            {
                sum = sum + AT[transactions[j]].get_tr(); // calculate amount of transactions
            }
        }
        sum = static_cast<double>(((stock - sum) + stock) / 2) * 0.1; // average of stock in 7 days
        if (sum <= 0)
        {
            throw invalid_argument("This user can not take profit!");
        }
        this->transaction(sum, DEPOSIT, v_customer,AT); // call deposit method
        banktransaction temp(sum);                   // make new transaction obj
        temp.set_source(this->get_username());       // store source account name
        temp.set_status(PROFIT);                     // for checking date and add profit next time
        AT.push_back(temp);                          // store transaction in vector
        this->save_transactions(AT);                 // store number of vector<banktransaction> in transaction list customer
        cout << "Profit successfully added" << endl;
    }
    catch (const invalid_argument &w)
    {
        std::cerr << w.what() << endl;
        return false;
    }
    catch (const out_of_range &w)
    {
        cerr << w.what() << endl;
        return false;
    }
    return true;
}