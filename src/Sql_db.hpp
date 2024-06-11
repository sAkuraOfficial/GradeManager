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
    std::string lesson_name = "";     // 授课名称
    std::string class_id = "";        // 授课对应班级
    std::string course_id = "";       // 课程id
    std::string teacher_id = "";      // 教师id
    float grade_daily_percent = 0.0f; // 平时成绩占比
};

struct student_
{
    std::string student_name = "";
    std::string student_id = "";
};

struct grade
{
    std::string student_class = ""; // 学生班别
    std::string student_id = "";    // 学生学号
    std::string student_name = "";  // 学生姓名
    std::string course_name = "";   // 学科名
    int course_semester = 0;        // 学科学期
    float grade_daily = 0.0f;       // 平时分
    float grade_final = 0.0f;       // 期末分
    float grade_total = 0.0f;       // 总分
};

class Sql_db
{
  private:
    SQLite::Database *db;

  public:
    Sql_db();
    ~Sql_db();

    // 登录
    bool login(std::string user_id, std::string user_pwd, std::string &user_name);

    // 注册
    bool reg(std::string user_id, std::string user_pwd, std::string user_name);

    // 获取教师的授课
    bool get_teacher_lesson(std::string user_id, std::vector<lesson> &courses);

    // bool get_all_college(std::vector<college> &college);

    // 获取所有专业
    bool get_all_major(std::vector<major> &major);

    // 获取某课程的学生
    bool get_course_student(std::string class_id, std::vector<student_> &students);

    // 设置学生的某课程成绩
    bool set_student_course_grade(std::string student_id, std::string course_id, float grade_daily, float grade_final, float grade_total);

    // 获取学生的某课程成绩
    bool get_student_course_grade(std::string student_id,std::string course_id ,grade &grade);

    // 获取学生所有成绩
    bool get_student_all_grade(std::string student_id, std::vector<grade> &grades);
};
