#ifndef CUSTOMER_HPP
#define CUSTOMER_HPP
#include <iostream>
#include <vector>
#include <stdbool.h>
#include "banktransaction.hpp"
using namespace std;

// using for diffrent transactions
enum transaction
{
    DEPOSIT,
    WITHDRAW
};
class customer
{
private:
    static int customer_count;
    string username;
    int card_number;
    struct date
    {
        unsigned int year;
        unsigned int month;
        unsigned int day;
    };
    struct date array[2]; // 1st column : store opening date------- 2nd column  : store expiration date
    int stock;
    int borrow;
    vector<int> transactions; // store transaction for each customer
    vector<string> ip;        // store ip for each customer
public:
    customer();
    // set datas
    void set_name(string);
    void set_cadr_num(int);
    void set_stock();
    void set_borrow(int);
    bool set_ip(string);
    void set_date();

    // get datas
    string get_username();
    int get_card_num();
    int get_stock();
    int get_borrow();
    int get_count(); // return customers count
    void get_ip(); // return the IP
    string get_opening_date();
    string get_expiration_date();
    int get_size_transactions();

    // other functions can use
    bool transaction(int, int, vector<customer> &, vector<banktransaction> &) ;
    bool checkIP(string);
    void save_transactions(vector<banktransaction> &);
    bool take_profites(vector<banktransaction> &, vector<customer> &);
};

#endif