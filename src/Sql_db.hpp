#pragma once
#include <SQLiteCpp/SQLiteCpp.h>
#include <iostream>
#include <string>
class Sql_db
{
  private:
    SQLite::Database *db;

  public:
    Sql_db();
    ~Sql_db();
    bool login(std::string user_id, std::string user_pwd, std::string &user_name);
    bool reg(std::string user_id,std::string user_pwd,std::string user_name);
};
