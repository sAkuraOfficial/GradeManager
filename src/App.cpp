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
    input2_option.on_enter = [&] {
        if (account.user_login(user_id, user_pwd))
        {
            main_menu();
        }
        else
        {
            message = "登录失败，请检查账号密码";
        }
    };

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
        "成绩查询",
        "成绩编辑",
        "数据看板",
        "导出数据",
        "退出系统"
    };

    auto main_menu = Menu(&menu_items, &menu_items_selected, MenuOption::HorizontalAnimated());

    auto main_menu_tab = Container::Tab(
        {
            menu_grade_add(),
            menu_grade_search(&screen),
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
    static float show_grade_total = 0.0f;            // 用于在标题中显示总分
    static bool self_course_checked = true;          // 默认只显示自己的科目

    account.get_user_id(user_id);
    if (self_course_checked)
        sql_db->get_teacher_lesson(user_id, result_course);
    else
        sql_db->get_teacher_lesson("%", result_course);

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
            sql_db->get_course_student(result_course[lesson_select].class_id, result_student);

            student_id_select = 0;
            dropdown2_text.clear();
            for (auto &i : result_student)
            {
                dropdown2_text.push_back(i.student_id + "-" + i.student_name);
            }
        }
        else
        {
            student_id_select = 0;
            dropdown2_text.clear();
            dropdown2_text.push_back("请选择...");
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

    CheckboxOption checkbox1_option;
    checkbox1_option.on_change = [&]() {
        lesson_select = 0;
        student_id_select = 0;

        // 重新获取课程列表
        result_course.clear();
        dropdown1_text.clear();
        if (self_course_checked)
            sql_db->get_teacher_lesson(user_id, result_course);
        else
            sql_db->get_teacher_lesson("%", result_course);
        for (auto &i : result_course)
        {
            dropdown1_text.push_back(i.lesson_name);
        }

        // 重置学生列表
        result_student.clear();
        dropdown2_text.clear();
        dropdown2_text.push_back("请选择...");

        input1_value = "";
        input2_value = "";
        show_grade_total = 0.0f;
        result_message = "请输入...";
    };
    checkbox1_option.transform = [&](const EntryState &s) {
        auto prefix = text(s.state ? "✅ " : "❎ "); // NOLINT
        auto t = text(s.label);
        if (s.active)
        {
            t |= bold;
        }
        if (s.focused)
        {
            t |= inverted;
        }
        return hbox({prefix, t});
    };

    static auto checkbox1 = Checkbox("仅显示自身授课", &self_course_checked, checkbox1_option);

    // clang-format off
    // 这里关闭了格式化,因为格式化会破坏代码的美观
    // 平时分输入
    InputOption input_option;
    input_option.multiline=false;
    input_option.on_change = [&]() {
        // 只允许输入数字和小数点
        input1_value.erase(
            std::remove_if(
                input1_value.begin(),input1_value.end(),
                [](char c) {return !std::isdigit(c) && c != '.';}
            ),
            input1_value.end()
        );
         input2_value.erase(
            std::remove_if(
                input2_value.begin(),input2_value.end(),
                [](char c) {return !std::isdigit(c) && c != '.';}
            ),
            input2_value.end()
        );
        //每次输入后,自动计算总分
        float grade_daily_percent = result_course[lesson_select].grade_daily_percent;
        float grade_daily = input1_value== "" ? 0.0f: std::stof(input1_value);//防止出错
        float grade_final = input2_value== "" ? 0.0f: std::stof(input2_value);
        show_grade_total = grade_daily * grade_daily_percent + grade_final * (1 - grade_daily_percent);
    };
    static auto input1 = Input(&input1_value, "请输入平时分", input_option) | size(WIDTH, EQUAL, 20);
    // 期末分输入
    static auto input2 = Input(&input2_value, "请输入期末分", input_option) | size(WIDTH, EQUAL, 20);
    // clang-format on

    static auto button1 = Button("重新输入", [&]() {
        input1_value = "";
        input2_value = "";
    });
    static auto button2 = Button("确认输入", [&]() {
        if (lesson_select != 0 && student_id_select != 0)
        {
            if (input1_value == "" || input2_value == "")
            {
                result_message = "请输入完整的成绩信息!";
                return;
            }
            // 通过比例计算总分:
            float grade_daily_percent = result_course[lesson_select].grade_daily_percent;
            float grade_daily = input1_value == "" ? 0.0f : std::stof(input1_value);
            float grade_final = input2_value == "" ? 0.0f : std::stof(input2_value);
            float grade_total = grade_daily * grade_daily_percent + grade_final * (1 - grade_daily_percent);

            // 插入/覆盖记录到数据库
            std::string student_id_ = result_student[student_id_select].student_id;
            std::string course_id = result_course[lesson_select].course_id;
            sql_db->set_student_course_grade(student_id_, course_id, grade_daily, grade_final, grade_total);

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
        dropdown_1, // 科目列表
        dropdown_2, // 学生列表
        input1,     // 平时分
        input2,     // 期末分
        checkbox1,  // 选择框:仅自己科目
        button1,    // 重新输入
        button2,    // 确认输入
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
                hbox({text(result_message), filler(), checkbox1->Render()}),
                separator(),
                hbox({text("😍 总分: "), text(std::to_string(show_grade_total)) | color(Color::Blue)}) | center,
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

Component App::menu_grade_search(Screen *screen)
{
    // 静态标志变量
    static int dropdown1_select = 0;

    // 获取当前教师id
    static std::string teacher_id = "";
    account.get_user_id(teacher_id);

    // 获取默认课程
    static std::vector<lesson> result_course;
    sql_db->get_teacher_lesson(teacher_id, result_course);

    // 课程的所有学生
    static std::vector<student_> result_student;

    // 课程的所有学生的成绩
    static std::vector<grade> result_grade;

    // 设置下拉框标题:所有课程
    static std::vector<std::string> dropdown1_text;
    for (auto &i : result_course)
    {
        dropdown1_text.push_back(i.lesson_name);
    }

    // 下拉框
    static DropdownOption dropdown1_option;
    dropdown1_option.radiobox.entries = &dropdown1_text;
    dropdown1_option.radiobox.selected = &dropdown1_select;
    dropdown1_option.radiobox.on_change = [&]() {
        if (dropdown1_select != 0)
        {
            sql_db->get_course_student(result_course[dropdown1_select].class_id, result_student);
            result_student.erase(result_student.begin());
            result_grade.clear();
            for (auto &i : result_student)
            {
                grade temp;
                sql_db->get_student_course_grade(i.student_id, result_course[dropdown1_select].course_id, temp);
                result_grade.push_back(temp);
            }
        }
    };

    dropdown1_option.transform = [](bool open, Element checkbox, Element radiobox) {
        if (open)
            return vbox({
                checkbox | inverted,
                radiobox | vscroll_indicator | frame |
                    size(HEIGHT, LESS_THAN, 5),
                filler(),
            });
        return vbox({checkbox, filler()});
    };

    static auto dropdown1 = Dropdown(dropdown1_option);

    static auto childs = Container::Vertical({dropdown1});

    static auto renderer = Renderer(childs, [&]() {
        Elements childrens = {
            dropdown1->Render(),
            separator(),
        };

        //创建表头数据
        Elements table_head = {
            text("班级"),
            separator(),
            text("学号姓名"),
            separator(),
            text("平时分"),
            separator(),
            text("期末分"),
            separator(),
            text("总分"),
        };

        std::vector<Elements> elements_arr;
        elements_arr.push_back(table_head);

        // 创建表格的数据
        for (auto &i : result_student)
        {
            if (i.student_id == "" || i.student_name == "")
                continue;
            grade temp_grade;
            if (!sql_db->get_student_course_grade(i.student_id, result_course[dropdown1_select].course_id, temp_grade))
            {
                continue; // 读取失败
            }
            Elements temp_elements = {
                text(temp_grade.student_class),
                separator(),
                text(i.student_id + "-" + i.student_name),
                separator(),
                text(std::to_string(temp_grade.grade_daily)),
                separator(),
                text(std::to_string(temp_grade.grade_final)),
                separator(),
                text(std::to_string(temp_grade.grade_total)),
            };
            elements_arr.push_back(temp_elements);
        }

        // 创建table
        auto table_ = Table(elements_arr);
        table_.SelectAll().Border(LIGHT);
        table_.SelectAll().SeparatorHorizontal(LIGHT);
        auto r = table_.Render();
        childrens.push_back(vbox({r}) | center);

        auto box = vbox({childrens});
        box |= border;
        box |= size(WIDTH, EQUAL, ftxui::Terminal::Size().dimx);
        box |= size(HEIGHT, EQUAL, ftxui::Terminal::Size().dimy);
        box |= center;

        return box;
    });
    return renderer;
}
