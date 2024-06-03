#include "Sql_db.hpp"

Sql_db::Sql_db()
{
    try
    {
        db = new SQLite::Database("GradeManager.db", SQLite::OPEN_READWRITE);
    }
    catch (std::exception &e)
    {
        std::cout << "exception: " << e.what() << std::endl;
    }
}
Sql_db::~Sql_db()
{
}
bool Sql_db::login(std::string user_id, std::string user_pwd, std::string &user_name)
{

    SQLite::Statement query(*db, "SELECT * FROM Account WHERE user_id = ? AND user_pwd = ?");

    query.bind(1, user_id);
    query.bind(2, user_pwd);

    if (query.executeStep())
    {
        std::string name = query.getColumn("user_name");
        user_name = name;
        return true;
    }
    else
    {
        return false;
    }
}
bool Sql_db::reg(std::string user_id, std::string user_pwd, std::string user_name)
{
    SQLite::Statement query(*db, "INSERT INTO Account(user_id,user_pwd,user_name) VALUES(?,?,?)");

    query.bind(1, user_id);
    query.bind(2, user_pwd);
    query.bind(3, user_name);
    try
    {
        int result = query.exec(); // result是插入的行数
        // 若主键(user_id)冲突,将进入catch中
        return result;
    }
    catch (std::exception &e)
    {
        std::cout << "exception: " << e.what() << std::endl;
        return false;
    }
}
