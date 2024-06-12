#pragma once
#include "ftxui/component/captured_mouse.hpp"
#include "ftxui/component/component.hpp"
#include "ftxui/component/component_base.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/elements.hpp"
#include "ftxui/dom/table.hpp"
#include <iostream>

#include "Account.hpp"
#include "Sql_db.hpp"
using namespace ftxui;
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
    Component menu_grade_add();
    Component menu_grade_search(Screen* screen);
};

enum sex
{
    noneSex,
    male,
    female
};

enum user_type
{
    noneType,
    student,
    teacher,
};