#pragma once
#include <SQLiteCpp/SQLiteCpp.h>
#include <iostream>
#include <string>
#include <vector>

struct grade;
struct rank;

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

struct lesson // 这是授课,和下面的班级不一样
{
    std::string lesson_name = "";     // 授课名称
    std::string class_id = "";        // 授课对应班级
    std::string course_id = "";       // 课程id
    std::string teacher_id = "";      // 教师id
    float grade_daily_percent = 0.0f; // 平时成绩占比
};

struct class_
{
    std::string class_name = "";        // 如22计科7班
    std::string class_id = "";          // 班级id
    int stu_num = 0;                    // 学生人数
    std::vector<grade> students_grades; // 所有学生的所有成绩
    std::vector<rank> students_ranks;   // 所有学生的所有总分排名
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

struct rank
{
    std::string student_name = ""; // 学生姓名
    std::string student_id = "";   // 学生学号
    float grade_total = 0.0f;      // 学生总分
    int class_rank = 0;            // 班级排名
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
    bool get_student_course_grade(std::string student_id, std::string course_id, grade &grade);

    // 获取学生所有成绩
    bool get_student_all_grade(std::string student_id, std::vector<grade> &grades);

    // 获取某个班级的所有学生的总分排名
    // 提供两种模式,1.只返回该学生的排名,2.返回所有学生的排名
    bool get_class_student_rank(std::string student_id, bool only_this_student, std::vector<rank> &ranks);

    // (重载函数)获取某个班级的所有学生的总分排名
    bool get_class_student_rank(std::string class_id, std::vector<rank> &ranks);

    // 获取某个学生的平均分
    bool get_student_avg_grade(std::string student_id, float &avg_grade);

    // 获取所有班级
    bool get_all_class_info(std::vector<class_> &classes);
};
