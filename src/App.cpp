#include "App.hpp"
App::App()
    : sql_db(new Sql_db()), account(sql_db)
{
}

App::~App()
{
}

void App::login()
{
    auto screen = ScreenInteractive::Fullscreen();
    screen.SetCursor(ftxui::Screen::Cursor(0, 0, ftxui::Screen::Cursor::Hidden));
    std::string user_id, user_pwd;
    std::string message = "请登录......";
    bool reg_result = false;

    auto btn1 = Button(
        "登录", [&] {
        if (account.user_login(user_id, user_pwd))
        {
            main_menu();
        }
        else
        {
            message = "登录失败，请检查账号密码";
        }
    }
    );
    auto btn2 = Button(
        "注册", [&] {
        reg(reg_result);
        if (reg_result)
        {
            message = "注册成功，请登录";
        }
    }
    );
    auto btn3 = Button(
        "退出", [&] {
        screen.Exit();
    }
    );

    // test button login
    auto btn4 = Button(
        "测试登录", [&] {
        if (account.user_login("1111", "123456"))
        {
            main_menu();
        }
    }
    );

    InputOption input2_option;
    input2_option.password = true;
    input2_option.multiline = false;

    auto input1 = Input(&user_id, "请输入账号", {.multiline = false}) | size(WIDTH, EQUAL, 20);
    auto input2 = Input(&user_pwd, "请输入密码", input2_option) | size(WIDTH, EQUAL, 20);

    auto childs = Container::Vertical({input1, input2, btn1, btn2, btn3, btn4});

    auto component = Renderer(childs, [&] {
        auto child_box1 = vbox(
            {
                hbox({text("欢迎使用学生成绩管理系统")}) | center,
                separator(),
                hbox({text("账号: "), input1->Render()}) | center,
                hbox({text("密码: "), input2->Render()}) | center,
                text(message),
                separator(),
                hbox({
                    btn1->Render(),
                    btn2->Render(),
                    btn3->Render(),
                    btn4->Render(),
                }) | center,
            }
        );
        child_box1 |= border | size(WIDTH, EQUAL, 40) | center;

        auto father_box = vbox(
            {
                filler(),
                child_box1,
                filler(),
            }
        );
        return father_box;
    });

    screen.Loop(component);
}

void App::reg(bool &reg_result)
{
    auto screen = ScreenInteractive::Fullscreen();
    screen.SetCursor(ftxui::Screen::Cursor(0, 0, ftxui::Screen::Cursor::Hidden));

    bool user_type = false;

    std::string user_id, user_pwd, user_name, citizen_id;
    std::string reg_message = "请注册......";

    std::vector<std::string> sex_text = {"男性", "女性"};
    std::vector<std::string> user_type_text = {"学生", "教师"};

    int sex_selected = 0;
    int type_selected = 0;

    auto btn1 = Button(
        "确认注册", [&] {
        if (account.user_register(user_id, user_pwd, user_name))
        {
            screen.Exit();
            reg_result = true;
        }
        else
        {
            reg_message = "注册失败，账号重复";
        }
    }
    );
    auto btn2 = Button(
        "返回", [&] {
        screen.Exit();
        reg_result = false;
    }
    );

    btn1 |= size(WIDTH, EQUAL, 15) | center;
    btn2 |= size(WIDTH, EQUAL, 15) | center;

    InputOption input2_option;
    input2_option.password = true;
    input2_option.multiline = false;

    auto input1 = Input(&user_id, "请输入账号", {.multiline = false}) | size(WIDTH, EQUAL, 20);
    auto input2 = Input(&user_pwd, "请输入密码", input2_option) | size(WIDTH, EQUAL, 20);
    auto input3 = Input(&user_name, "请输入姓名", {.multiline = false}) | size(WIDTH, EQUAL, 20);
    auto input4 = Input(&citizen_id, "请输入身份证号", {.multiline = false}) | size(WIDTH, EQUAL, 20);

    auto radiobox1 = Radiobox(sex_text, &sex_selected);
    auto radiobox2 = Radiobox(user_type_text, &type_selected);

    auto childs = Container::Vertical({input1, input2, input3, input4, radiobox1, radiobox2, btn1, btn2});

    auto component = Renderer(childs, [&] {
        auto child_box1 = vbox(
            {
                hbox({text("注册账号")}) | center,
                separator(),
                vbox(
                    {
                        hbox({text("账号: "), input1->Render()}) | center,
                        separator(),
                        hbox({text("密码: "), input2->Render()}) | center,
                        separator(),
                        hbox({text("姓名: "), input3->Render()}) | center,
                        separator(),
                        hbox({text("身份证号: "), input4->Render()}) | center,
                        separator(),
                        hbox({text("性别: "), radiobox1->Render(), separator(), text("类型: "), radiobox2->Render()}) | center,
                    }
                ) | border,
                text(reg_message),
                separator(),
                hbox({btn1->Render(), btn2->Render()}) | center,
            }
        );
        child_box1 |= border | size(WIDTH, EQUAL, 40) | center;

        auto father_box = vbox(
            {
                filler(),
                child_box1,
                filler(),
            }
        );

        return father_box;
    });

    screen.Loop(component);
}
void App::main_menu()
{
    // auto screen = ScreenInteractive::TerminalOutput();
    auto screen = ScreenInteractive::Fullscreen();
    screen.SetCursor(ftxui::Screen::Cursor(0, 0, ftxui::Screen::Cursor::Hidden)); // 隐藏终端的光标

    int menu_items_selected = 0;
    std::vector<std::string> menu_items = {
        "成绩录入",
        "编辑删除",
        "成绩查询",
        "导出数据",
        "退出系统"
    };

    auto main_menu = Menu(&menu_items, &menu_items_selected, MenuOption::HorizontalAnimated());

    auto main_menu_tab = Container::Tab(
        {
            menu_grade_add(),
        },
        &menu_items_selected
    );

    auto childs = Container::Vertical({main_menu, main_menu_tab});

    auto component = Renderer(childs, [&] {
        std::string user_name = "";
        account.get_user_name(user_name);
        auto father_box = vbox(
            {
                hbox({
                    text("成绩管理系统"),
                    text("     "),
                    text("当前登录账户:"),
                    text(user_name) | color(Color::Yellow),
                }) | center,
                separator(),
                vbox({main_menu->Render()}) | center,
                filler(),
                vbox({main_menu_tab->Render()}),
                filler(),
            }
        );
        return father_box | border;
    });
    screen.Loop(component);
}

Component App::menu_grade_add()
{
    // 备注:因为为了代码美观,menu_grade_add是包含在main_menu中的,不是单独的screen,
    // 因此,当前函数变量的生命周期不能短过main_menu的生命周期,所以我给变量加了static
    static int lesson_select = 0;                    // 课程下拉框索引,id从1开始,0代表未选择
    static int student_id_select = 0;                // 学生下拉框索引,id从1开始,0代表未选择
    static std::string input1_value;                 // 存储平时分
    static std::string input2_value;                 // 存储期末分
    static std::vector<lesson> result_course;        // 在下面初始化
    static std::vector<student_> result_student;     // 在下面初始化
    static std::vector<std::string> dropdown1_text;  // course
    static std::vector<std::string> dropdown2_text;  // lesson
    static std::string user_id = "";                 // 用于存储教师id
    static student_ student_selected = {};           // 用于存储选中了的学生信息
    static std::string result_message = "请输入..."; // 通知

    account.get_user_id(user_id);
    sql_db->get_user_courses(user_id, result_course);

    for (auto &i : result_course)
    {
        dropdown1_text.push_back(i.lesson_name);
    }
    dropdown2_text.push_back("请选择...");

    // 选择科目:
    static auto dropdown_1 = Dropdown({
        .radiobox = {
                     .entries = &dropdown1_text,
                     .selected = &lesson_select,
                     .on_change = [&]() {
        if (lesson_select != 0)
        {
            sql_db->get_course_student(result_course[lesson_select].class_id, result_student); //
            student_id_select = 0;
            if (!dropdown2_text.empty())//这样处理的原因是因为ftxui库为异步渲染,直接清空会导致访问越界,因此保留第一个"请选择"提示语
            {
                dropdown2_text.erase(dropdown2_text.begin() + 1, dropdown2_text.end());
            }
            for (int i = 1; i < result_student.size(); i++)//第一个为"请选择"提示语,直接跳过
            {
                dropdown2_text.push_back(result_student[i].student_id + "-" + result_student[i].student_name);
            }
        }
        else
        {
            student_id_select = 0;
            if (!dropdown2_text.empty())
            {
                dropdown2_text.erase(dropdown2_text.begin() + 1, dropdown2_text.end());
            }
        }
    }
        },
        .transform = [](bool open, Element checkbox, Element radiobox) {
        if (open)
        {

            return vbox({
                checkbox | inverted,
                radiobox | vscroll_indicator | frame |
                    size(HEIGHT, LESS_THAN, 5),
                filler(),
            });
        }
        return vbox({
            checkbox,
            filler(),
        });
                     },
    });

    // 二级联动,据选择的科目,获取学生列表
    static auto dropdown_2 = Dropdown({
        .radiobox = {
                     .entries = &dropdown2_text,
                     .selected = &student_id_select,
                     //.on_change = [&]() {}
        },
        .transform = [](bool open, Element checkbox, Element radiobox) {
        if (open)
        {

            return vbox({
                checkbox | inverted,
                radiobox | vscroll_indicator | frame |
                    size(HEIGHT, LESS_THAN, 5),
                filler(),
            });
        }
        return vbox({
            checkbox,
            filler(),
        });
                     },
    });

    // 平时分输入

    static auto input1 = Input(&input1_value, "请输入平时分", {.multiline = false}) | size(WIDTH, EQUAL, 20);
    // 期末分输入
    static auto input2 = Input(&input2_value, "请输入期末分", {.multiline = false}) | size(WIDTH, EQUAL, 20);

    static auto button1 = Button("重新输入", [&]() {
        input1_value = "";
        input2_value = "";
    });
    static auto button2 = Button("确认输入", [&]() {
        if (lesson_select != 0 && student_id_select != 0)
        {

            // 通过比例计算总分:
            float grade_daily_percent = result_course[lesson_select].grade_daily_percent;
            float grade_daily = std::stof(input1_value);
            float grade_final = std::stof(input2_value);
            float grade_total = grade_daily * grade_daily_percent + grade_final * (1 - grade_daily_percent);

            // 插入/覆盖记录到数据库
            std::string student_id_ = result_student[student_id_select].student_id;
            std::string course_id = result_course[lesson_select].course_id;
            sql_db->set_student_grade(student_id_, course_id, grade_daily, grade_final, grade_total);

            // 清空输入框
            input1_value = "";
            input2_value = "";

            // 切换到下一个学生
            if (student_id_select + 1 < result_student.size())
            {
                result_message = "输入成功!已智能为您切换到下一个学生!";
                student_id_select++;
            }
            else
            {
                result_message = "输入成功!已经是最后一个学生!";
            }
        }
        else
        {
            result_message = "您还没有选择科目或学号,请重新选择";
            return;
        }
    });

    auto childs = Container::Vertical({
        dropdown_1,
        dropdown_2,
        input1,
        input2,
        button1,
        button2,
    });

    auto renderer = Renderer(childs, [&] {
        auto father_box = vbox(
            {
                text("信息输入") | center,
                separator(),
                hbox({text("📚 请选择科目: "), dropdown_1->Render()}),
                hbox({text("🆔 请选择学号: "), dropdown_2->Render()}),
                separator(),
                hbox({text("💯 输入平时分: "), input1->Render()}),
                hbox({text("📝 输入期末分: "), input2->Render()}),
                text("系统将按照学科预设的比例计算总分") | center,
                text(result_message),
                separator(),
                hbox({text("😍 总分: "), text(std::to_string(lesson_select)) | color(Color::Blue)}) | center,
                hbox({button1->Render(), button2->Render()}) | center,
            }
        );
        father_box |= border;
        father_box |= size(WIDTH, EQUAL, 60);
        father_box |= center;

        return father_box;
    });

    return renderer;
}
