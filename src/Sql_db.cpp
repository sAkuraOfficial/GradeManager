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

// 登录
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

// 注册
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

bool Sql_db::get_user_courses(std::string user_id, std::vector<lesson> &courses)
{
    try
    {
        std::string sql_query =
            "SELECT\
                (SUBSTR(CAST(Class_.entry_year AS TEXT),3)) || Major.major_name || Class_.id || '班 - ' || Course.course_name AS 'lesson', \
				lesson.class_id AS 'class_id',\
                Course.course_id AS 'course_id',\
                Course.grade_daily_percent AS 'grade_daily_percent'\
            FROM\
                Course,	lesson,	Major,	Class_\
			WHERE\
	            lesson.teacher_id = ?\
                AND lesson.class_id = Class_.class_id\
                AND lesson.course_id = Course.course_id\
                AND Class_.major_id = Major.major_id\
            ";
        SQLite::Statement query(*db, sql_query);
        query.bind(1, user_id);
        std::vector<lesson> result_courses;
        result_courses.push_back({"请选择..", ""});
        while (query.executeStep())
        {
            std::string course_name = query.getColumn("lesson");
            std::string class_id = query.getColumn("class_id");
            std::string course_id = query.getColumn("course_id");
            float grade_daily_percent = query.getColumn("grade_daily_percent").getDouble();
            result_courses.push_back({course_name, class_id, course_id, grade_daily_percent});
        }
        courses = result_courses;
        return true;
    }
    catch (const std::exception &e)
    {
        std::vector<lesson> result_courses;
        result_courses.push_back({"未查询到当前账号的授课!", ""});
        courses = result_courses;
        return false;
    }
}

//// 获取所有学院
// bool Sql_db::get_all_college(std::vector<college> &college)
//{
//     try
//     {
//         SQLite::Statement query(*db, "SELECT * FROM College");
//         std::vector<struct college> result_college;
//         while (query.executeStep())
//         {
//             struct college temp;
//             temp.college_name = query.getColumn("college_name").getString();
//             temp.college_id = query.getColumn("college_id").getInt();
//             result_college.push_back(temp);
//         }
//         college = result_college;
//         return true;
//     }
//     catch (const std::exception &)
//     {
//         return false;
//     }
// }

// 获取所有专业
bool Sql_db::get_all_major(std::vector<major> &major)
{
    try
    {
        SQLite::Statement query(*db, "SELECT * FROM Major");
        std::vector<struct major> result_major;
        while (query.executeStep())
        {
            struct major temp;
            temp.major_name = query.getColumn("major_name").getString();
            temp.major_id = query.getColumn("major_id").getInt();
            result_major.push_back(temp);
        }
        major = result_major;
        return true;
    }
    catch (const std::exception &)
    {
        return false;
    }
}

// 根据lesson获取学生
bool Sql_db::get_course_student(std::string class_id, std::vector<student_> &students)
{
    try
    {
        std::string sql_query = "\
            SELECT\
                Account.user_id,Account.user_name\
            FROM Student,Account\
            WHERE\
                Student.class_id = ? AND\
                Student.user_id = Account.user_id\
            ";
        SQLite::Statement query(*db, sql_query);
        query.bind(1, class_id);
        std::vector<struct student_> result_students;
        result_students.push_back({"请选择...", ""});
        while (query.executeStep())
        {
            struct student_ temp;
            temp.student_id = query.getColumn("user_id").getString();
            temp.student_name = query.getColumn("user_name").getString();
            result_students.push_back(temp);
        }
        students = result_students;
        return true;
    }
    catch (const std::exception &e)
    {
        return false;
    }
}

bool Sql_db::set_student_grade(std::string student_id, std::string course_id, float grade_daily, float grade_final, float grade_total)
{
    try
    {
        std::string sql_query = "\
			INSERT OR REPLACE INTO Grade\
			(student_id,course_id,grade_daily,grade_final,grade_total)\
			VALUES(?,?,?,?,?)\
			";
        SQLite::Statement query(*db, sql_query);
        query.bind(1, student_id);
        query.bind(2, course_id);
        query.bind(3, grade_daily);
        query.bind(4, grade_final);
        query.bind(5, grade_total);
        query.exec();
        return true;
    }
    catch (const std::exception &e)
    {
        return false;
    }
}
