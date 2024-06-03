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
    return sql_db->login(user_id, user_pwd, user_name);
}

bool Account::user_register(std::string id, std::string pwd, std::string name)
{
    user_id = id;
    user_pwd = pwd;
    user_name = name;
    return sql_db->reg(user_id, user_pwd, user_name);
}
