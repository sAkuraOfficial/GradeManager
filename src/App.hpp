#pragma once
#include "Account.hpp"
#include "Sql_db.hpp"
#include "ftxui/component/captured_mouse.hpp"
#include "ftxui/component/component.hpp"
#include "ftxui/component/component_base.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/elements.hpp"
#include "ftxui/dom/table.hpp"
#include <iostream>

using namespace ftxui;

struct message_grade_edit; // 声明message结构体
enum class message_list_menu_grade_edit;

class App
{
  private:
    Sql_db *sql_db;
    Account account;

  public:
    App();
    ~App();
    void login();
    void reg(bool &reg_result);
    void main_menu();
    Component menu_grade_edit();
    Component menu_grade_get();
    message_grade_edit get_message_menu_grade_edit(message_list_menu_grade_edit message_id);
};

enum class sex
{
    noneSex,
    male,
    female
};

enum class user_type
{
    noneType,
    student,
    teacher,
};

enum class message_list_menu_grade_edit // 编辑
{
    NONE,                        // 未定义
    INPUT_ERROR,                 // 输入错误
    WAIT_INPUT,                  // 请输入
    READ_SUCCESSED,              // 读取成功
    READ_FAILED,                 // 读取失败
    DELETE_SUCCESSED,            // 删除成功
    DELETE_FAILED,               // 删除失败
    WRITE_SUCCESSED_SWITCH_NEXT, // 写入成功,切换到下一个
    WRITE_SUCCESSED_LAST,        // 最后一个学生写入成功
    WRITE_FAILED,                // 写入失败
};

enum class message_list_menu_grade_query // 查询
{
    NONE,            // 未定义
    INPUT_ERROR,     // 输入错误
    WAIT_INPUT,      // 请输入
    QUERY_SUCCESSED, // 获取成功
    QUERY_FAILED,    // 获取失败
};

struct message_grade_edit
{
    message_list_menu_grade_edit message_id = message_list_menu_grade_edit::NONE;
    std::string msg = "当前消息未定义!";
    ftxui::Color color = ftxui::Color::Default;
};

struct message_grade_query
{
    message_list_menu_grade_query message_id = message_list_menu_grade_query::NONE;
	std::string msg = "当前消息未定义!";
	ftxui::Color color = ftxui::Color::Default;
};

