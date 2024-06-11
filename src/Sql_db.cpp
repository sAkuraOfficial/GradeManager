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

bool Sql_db::get_teacher_lesson(std::string user_id, std::vector<lesson> &courses)
{
    try
    {
        std::string sql_query =
            "SELECT\
                (SUBSTR(CAST(Class_.entry_year AS TEXT),3)) || Major.major_name || Class_.id || '班 - ' || Course.course_name AS 'lesson', \
				lesson.class_id AS 'class_id',\
                Course.course_id AS 'course_id',\
                Course.grade_daily_percent AS 'grade_daily_percent',\
                lesson.teacher_id AS 'teacher_id'\
            FROM\
                Course,	lesson,	Major,	Class_\
			WHERE\
	            lesson.teacher_id LIKE ?\
                AND lesson.class_id = Class_.class_id\
                AND lesson.course_id = Course.course_id\
                AND Class_.major_id = Major.major_id\
            "; // 用了LIKE,user_id支持通配符
        SQLite::Statement query(*db, sql_query);
        query.bind(1, user_id);
        std::vector<lesson> result_courses;
        result_courses.push_back({"请选择...", ""});
        while (query.executeStep())
        {
            std::string course_name = query.getColumn("lesson");
            std::string class_id = query.getColumn("class_id");
            std::string course_id = query.getColumn("course_id");
            std::string teacher_id = query.getColumn("teacher_id");
            float grade_daily_percent = query.getColumn("grade_daily_percent").getDouble();
            result_courses.push_back({course_name, class_id, course_id, teacher_id, grade_daily_percent});
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

bool Sql_db::set_student_course_grade(std::string student_id, std::string course_id, float grade_daily, float grade_final, float grade_total)
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

bool Sql_db::get_student_course_grade(std::string student_id, std::string course_id, grade &grade)
{
    try
    {
        std::string sql_query = "\
        SELECT\
            (SUBSTR(CAST(Class_.entry_year AS TEXT),3))||'级'||Major.major_name||Class_.id||'班' AS 'class_',\
            Account.user_id AS 'user_id',\
            Account.user_name AS 'user_name',\
            Course.course_name AS 'course_name',\
            Student.semester AS 'semester',\
            grade.grade_daily AS 'grade_daily',\
            grade.grade_final AS 'grade_final',\
            grade.grade_total AS 'grade_total'\
        FROM\
            grade\
        INNER JOIN\
            Student ON grade.student_id = Student.user_id\
        INNER JOIN\
            Account ON Student.user_id = Account.user_id\
        INNER JOIN \
            Class_ ON Student.class_id = Class_.class_id\
        INNER JOIN \
            Major ON Class_.major_id = Major.major_id\
        INNER JOIN\
		        Course ON Course.course_id=grade.course_id\
        WHERE \
            Student.user_id = ? AND\
		    grade.course_id = ?\
        ";
        SQLite::Statement query(*db, sql_query);
        query.bind(1, student_id);
        query.bind(2, course_id);
        if (query.executeStep())
        {
            grade.student_class = query.getColumn("class_").getString();
            grade.student_id = query.getColumn("user_id").getString();
            grade.student_name = query.getColumn("user_name").getString();
            grade.course_name = query.getColumn("course_name").getString();
            grade.course_semester = query.getColumn("semester").getInt();
            grade.grade_daily = query.getColumn("grade_daily").getDouble();
            grade.grade_final = query.getColumn("grade_final").getDouble();
            grade.grade_total = query.getColumn("grade_total").getDouble();
            return true;
        }
        else
        {
            return false;
        }
    }
    catch (const std::exception &e)
    {
    }
}

bool Sql_db::get_student_all_grade(std::string student_id, std::vector<grade> &grades)
{
    try
    {
        std::string sql_query = "\
        SELECT\
            (SUBSTR(CAST(Class_.entry_year AS TEXT),3))||'级'||Major.major_name||Class_.id||'班' AS 'class_',\
            Account.user_id AS 'user_id',\
            Account.user_name AS 'user_name',\
            Course.course_name AS 'course_name',\
            Student.semester AS 'semester',\
            grade.grade_daily AS 'grade_daily',\
            grade.grade_final AS 'grade_final',\
            grade.grade_total AS 'grade_total'\
        FROM\
            grade\
        INNER JOIN\
            Student ON grade.student_id = Student.user_id\
        INNER JOIN\
            Account ON Student.user_id = Account.user_id\
        INNER JOIN \
            Class_ ON Student.class_id = Class_.class_id\
        INNER JOIN \
            Major ON Class_.major_id = Major.major_id\
        INNER JOIN\
		        Course ON Course.course_id=grade.course_id\
        WHERE \
            Student.user_id = ?\
        ";
        SQLite::Statement query(*db, sql_query);
        std::vector<grade> grades_temp;
        query.bind(1, student_id);
        while (query.executeStep())
        {
            grade temp;
            temp.student_class = query.getColumn("class_").getString();
            temp.student_id = query.getColumn("user_id").getString();
            temp.student_name = query.getColumn("user_name").getString();
            temp.course_name = query.getColumn("course_name").getString();
            temp.course_semester = query.getColumn("semester").getInt();
            temp.grade_daily = query.getColumn("grade_daily").getDouble();
            temp.grade_final = query.getColumn("grade_final").getDouble();
            temp.grade_total = query.getColumn("grade_total").getDouble();
            grades_temp.push_back(temp);
        }
        grades = grades_temp;
        return true;
    }
    catch (const std::exception &e)
    {
        return false;
    }
}
