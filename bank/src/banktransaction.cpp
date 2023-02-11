#include "../header/banktransaction.hpp"

void banktransaction::set_source(string s_s)
{
    source = s_s;
}
banktransaction::banktransaction(int amount)
{
    // calculate present time or date
    time_t now = time(0);
    tm *ltm = localtime(&now);
    year = ltm->tm_year + 1900; // set year from 1900
    month = 1 + ltm->tm_mon;
    day = ltm->tm_mday;
    tr = amount;
}
void banktransaction::set_destination(string s_d)
{
    destination = s_d;
}
void banktransaction::set_status(int enum_arg)
{
    //enum_parg = enum argumant
    status = enum_arg;
} 
void banktransaction::set_loan_status(int enum_arg)
{
    loan_status = enum_arg;
}
int banktransaction::get_day()
{
    return day;
}
int banktransaction::get_month()
{
    return month;
}
int banktransaction ::get_year()
{
    return year;
}
int banktransaction::get_tr()
{
    return tr;
}
// using for checking profit customers
int banktransaction::get_status()
{
    return status;
}
int banktransaction::get_loan_status()
{
    return loan_status;
}
string banktransaction::get_source()
{
    return source;
}
string banktransaction::get_destination()
{
    return destination;
}