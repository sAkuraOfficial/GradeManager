#pragma once
#include "Sql_db.hpp"
#include <string>
class Account
{
  private:
    std::string user_id;
    std::string user_pwd;
    std::string user_name;
    bool isLogin;
    Sql_db *sql_db;

  public:
    Account(Sql_db *db);
    ~Account();
    bool user_login(std::string id, std::string pwd);
    bool user_register(std::string id, std::string pwd, std::string name);
    bool get_teacher_lesson(std::vector<lesson> &courses);
    bool get_user_id(std::string& id);
    bool get_user_name(std::string& name);
    bool get_logined();
};
