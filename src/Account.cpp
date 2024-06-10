#include "Account.hpp"
Account::Account(Sql_db *sqldb)
    : user_id(), user_pwd(), user_name(), isLogin(false), sql_db(sqldb)
{
}

Account::~Account()
{
}

bool Account::user_login(std::string id, std::string pwd)
{
    user_id = id;
    user_pwd = pwd;
    isLogin = sql_db->login(user_id, user_pwd, user_name);
    return isLogin;
}

bool Account::user_register(std::string id, std::string pwd, std::string name)
{
    user_id = id;
    user_pwd = pwd;
    user_name = name;
    return sql_db->reg(user_id, user_pwd, user_name);
}

bool Account::get_user_courses(std::vector<lesson> &courses)
{
    return sql_db->get_user_courses(user_id, courses);
}

bool Account::get_user_id(std::string &id)
{
    if (isLogin)
    {
        id = user_id;
        return true;
    }
    else
    {
        return false;
    }
}

bool Account::get_user_name(std::string &name)
{
    if (isLogin)
    {
        name = user_name;
        return true;
    }
    else
    {
        return false;
    }
}

bool Account::get_logined()
{
    return isLogin;
}
