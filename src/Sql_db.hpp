#pragma once
#include <SQLiteCpp/SQLiteCpp.h>
#include <iostream>
#include <string>

struct college
{
    std::string college_name = "";
    int college_id = 0;
};

struct major
{
    std::string major_name = "";
    int major_id = 0;
};

struct lesson
{
    std::string lesson_name = ""; // 授课名称
    std::string class_id = "";    // 授课对应班级
    std::string course_id = "";   // 课程id
    float grade_daily_percent = 0.0f;
};

struct student_
{
    std::string student_name = "";
    std::string student_id = "";
};

class Sql_db
{
  private:
    SQLite::Database *db;

  public:
    Sql_db();
    ~Sql_db();
    bool login(std::string user_id, std::string user_pwd, std::string &user_name);
    bool reg(std::string user_id, std::string user_pwd, std::string user_name);
    bool get_user_courses(std::string user_id, std::vector<lesson> &courses);
    // bool get_all_college(std::vector<college> &college);
    bool get_all_major(std::vector<major> &major);
    bool get_course_student(std::string class_id, std::vector<student_> &students);
    bool set_student_grade(std::string student_id, std::string course_id, float grade_daily, float grade_final, float grade_total);
};
