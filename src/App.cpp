#include "App.hpp"
#include "css.hpp"
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
        "成绩编辑",
        "成绩查询",
        "导出数据",
        "退出系统"
    };

    auto main_menu = Menu(&menu_items, &menu_items_selected, MenuOption::HorizontalAnimated());

    auto main_menu_tab = Container::Tab(
        {
            menu_grade_edit(),
            menu_grade_get(),
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

Component App::menu_grade_edit()
{
    // 存储提示消息
    static message system_message = get_message_menu_grade_edit(message_menu_grade_edit::WAIT_INPUT);

    // 存储是否仅自身lesson
    static bool only_self_lesson = true;

    // 存储教师id
    static std::string user_id = ""; // 教师id
    account.get_user_id(user_id);

    // 存储平时分期末分的静态变量
    static float grade_daily = 0.0f;
    static float grade_final = 0.0f;
    static float grade_total = 0.0f;
    static std::string input1_value; // 平时分
    static std::string input2_value; // 期末分
    static std::string grade_total_text;

    // 学生下拉框
    static int student_select = 0;                                      // 下拉框索引
    static std::vector<student_> result_student;                        // 查询student结果
    static std::vector<std::string> dropdown2_text = {"未选择授课..."}; // 下拉框显示文本
    static auto dropdown2_option = dropdown_option_beautiful(&dropdown2_text, &student_select);
    static auto dropdown2 = Dropdown(dropdown2_option);

    // 科目下拉框
    static int lesson_select = 0;                   // 下拉框索引
    static std::vector<lesson> result_lesson;       // 查询lesson结果
    static std::vector<std::string> dropdown1_text; // 下拉框显示文本
    static auto dropdown1_option = dropdown_option_beautiful(&dropdown1_text, &lesson_select);
    dropdown1_option.radiobox.on_change = [&]() {
        student_select = 0;
        dropdown2_text.clear(); // 重置学生下拉框
        if (lesson_select)      // 选中非0,0是提示语
        {
            if (sql_db->get_course_student(result_lesson[lesson_select].class_id, result_student))
            {
                for (auto &i : result_student)
                    dropdown2_text.push_back(i.student_id + "-" + i.student_name);
            }
        }
        else
            dropdown2_text.push_back("未选择授课..."); // 重置学生下拉框
    };
    static auto dropdown1 = Dropdown(dropdown1_option);

    // 初始化科目,初始化下拉框文本
    if (only_self_lesson)
        sql_db->get_teacher_lesson(user_id, result_lesson);
    else
        sql_db->get_teacher_lesson("%", result_lesson);
    for (auto &i : result_lesson)
        dropdown1_text.push_back(i.lesson_name);

    // 自定义输入框设计
    static auto input_option = InputOption();
    input_option.multiline = false;
    input_option.on_change = [&]() {
        // 定义一个检测函数,检测输入是否为数字或小数点
        auto only_number = [](char c) {
            return !std::isdigit(c) && c != '.';
        };
        // 只允许输入数字和小数点
        input1_value.erase(std::remove_if(input1_value.begin(), input1_value.end(), only_number), input1_value.end());
        input2_value.erase(std::remove_if(input2_value.begin(), input2_value.end(), only_number), input2_value.end());
        // 每次输入后,自动计算总分
        float grade_daily_percent = result_lesson[lesson_select].grade_daily_percent;
        grade_daily = input1_value == "" ? 0.0f : std::stof(input1_value); // 防止出错,stof遇到空字符串会抛出异常
        grade_final = input2_value == "" ? 0.0f : std::stof(input2_value);
        grade_total = grade_daily * grade_daily_percent + grade_final * (1 - grade_daily_percent);
        grade_total_text = std::to_string(grade_total);
    };

    // 输入框_平时分和期末分
    static auto input1 = Input(&input1_value, "平时分...", input_option) | size(WIDTH, EQUAL, 20);
    static auto input2 = Input(&input2_value, "期末分...", input_option) | size(WIDTH, EQUAL, 20);

    // 选择框_是否仅自身授课
    static auto checkbox_option = checkbox_option_beautiful();
    checkbox_option.on_change = [&]() {
        lesson_select = student_select = 0;
        input1_value = input2_value = grade_total_text = "";
        system_message = get_message_menu_grade_edit(message_menu_grade_edit::WAIT_INPUT);
        dropdown1_text.clear();             // 重置科目下拉框
        dropdown2_text = {"未选择授课..."}; // 重置学生下拉框
        if (only_self_lesson)
            sql_db->get_teacher_lesson(user_id, result_lesson);
        else
            sql_db->get_teacher_lesson("%", result_lesson);
        for (auto &i : result_lesson)
            dropdown1_text.push_back(i.lesson_name);
    };
    static auto checkbox1 = Checkbox("仅显示自身授课", &only_self_lesson, checkbox_option);

    // 按钮_读取成绩,删除成绩,输入成绩
    static auto button1 = Button("读取成绩", [&]() {
        if (lesson_select == 0 || student_select == 0)
        {
            system_message = get_message_menu_grade_edit(message_menu_grade_edit::READ_FAILED);
            return;
        }
        grade temp_grade;
        bool read_ok = sql_db->get_student_course_grade(result_student[student_select].student_id, result_lesson[lesson_select].course_id, temp_grade);
        input1_value = read_ok ? std::to_string(temp_grade.grade_daily) : "";
        input2_value = read_ok ? std::to_string(temp_grade.grade_final) : "";
        grade_total = read_ok ? temp_grade.grade_total : 0.0f;
        grade_total_text = read_ok ? std::to_string(grade_total) : "";
        system_message = read_ok ? get_message_menu_grade_edit(message_menu_grade_edit::READ_SUCCESSED) : get_message_menu_grade_edit(message_menu_grade_edit::READ_FAILED);
    });

    static auto button2 = Button("删除成绩", [&]() {
        if (lesson_select == 0 || student_select == 0)
        {
            system_message = get_message_menu_grade_edit(message_menu_grade_edit::DELETE_FAILED);
            return;
        }
        if (sql_db->set_student_course_grade(result_student[student_select].student_id, result_lesson[lesson_select].course_id, 0.0f, 0.0f, 0.0f))
        {
            grade_daily = grade_final = grade_total = 0.0f; // 清空数据
            input1_value = input2_value = grade_total_text = "";
            system_message = get_message_menu_grade_edit(message_menu_grade_edit::DELETE_SUCCESSED);
        }
        else
            system_message = get_message_menu_grade_edit(message_menu_grade_edit::DELETE_FAILED); // 删除失败,则输入框保持原有数据
    });

    static auto button3 = Button("确认输入", [&]() {
        if (lesson_select == 0 || student_select == 0 || input1_value == "" || input2_value == "")
        {
            system_message = get_message_menu_grade_edit(message_menu_grade_edit::INPUT_ERROR);
            return;
        }
        bool write_ok = sql_db->set_student_course_grade(result_student[student_select].student_id, result_lesson[lesson_select].course_id, grade_daily, grade_final, grade_total);
        if (write_ok)
        {
            // 输入成功,自动切换到下一个学生
            if (student_select + 1 < result_student.size())
            {
                student_select++;
                grade temp_grade;
                bool read_ok = sql_db->get_student_course_grade(result_student[student_select].student_id, result_lesson[lesson_select].course_id, temp_grade);
                input1_value = read_ok ? std::to_string(temp_grade.grade_daily) : "";
                input2_value = read_ok ? std::to_string(temp_grade.grade_final) : "";
                grade_total = read_ok ? temp_grade.grade_total : 0.0f;
                grade_total_text = read_ok ? std::to_string(grade_total) : "";
                system_message = get_message_menu_grade_edit(message_menu_grade_edit::WRITE_SUCCESSED_SWITCH_NEXT);
            }
            else
                system_message = get_message_menu_grade_edit(message_menu_grade_edit::WRITE_SUCCESSED_LAST);
        }
        else
        {
            // 输入失败,保持原有数据
            system_message = get_message_menu_grade_edit(message_menu_grade_edit::WRITE_FAILED);
        }
    });

    // 子元素容器,用于提供给renderer接管输入事件
    static auto childs = Container::Vertical({dropdown1, dropdown2, input1, input2, checkbox1, button1, button2, button3});

    static auto renderer = Renderer(childs, [&]() {
        auto father_box = vbox({
            text("信息输入") | center,
            separator(),
            hbox({text("📚 请选择科目: "), dropdown1->Render()}),
            hbox({text("🆔 请选择学号: "), dropdown2->Render()}),
            separator(),
            hbox({text("💯 输入平时分: "), input1->Render()}),
            hbox({text("📝 输入期末分: "), input2->Render()}),
            text("系统将按照学科预设的比例计算总分") | center,
            hbox({text(system_message.msg) | color(system_message.color), filler(), checkbox1->Render()}),
            separator(),
            hbox({text("😍 总分: "), text(grade_total_text) | color(Color::Blue)}) | center,
            hbox({button1->Render(), button2->Render(), button3->Render()}) | center,
        });
        father_box |= border;
        father_box |= size(WIDTH, EQUAL, 60);
        father_box |= center;
        return father_box;
    });

    return renderer;
}

message App::get_message_menu_grade_edit(message_menu_grade_edit message_id)
{
    message result_message;
    result_message.message_id = message_id;
    if (message_id == message_menu_grade_edit::NONE)
    {
        result_message.msg = "👎🏻 当前消息未定义!";
        result_message.color = ftxui::Color::RedLight;
    }
    else if (message_id == message_menu_grade_edit::INPUT_ERROR)
    {
        result_message.msg = "🐷 信息输入不完整,请检查!";
        result_message.color = ftxui::Color::RedLight;
    }
    else if (message_id == message_menu_grade_edit::WAIT_INPUT)
    {
        result_message.msg = "😚 请输入...";
        result_message.color = ftxui::Color::YellowLight;
    }
    else if (message_id == message_menu_grade_edit::READ_SUCCESSED)
    {
        result_message.msg = "🥳 读取成功!";
        result_message.color = ftxui::Color::GreenLight;
    }
    else if (message_id == message_menu_grade_edit::READ_FAILED)
    {
        result_message.msg = "😅 读取失败!";
        result_message.color = ftxui::Color::RedLight;
    }
    else if (message_id == message_menu_grade_edit::DELETE_SUCCESSED)
    {
        result_message.msg = "🥳 删除成功!";
        result_message.color = ftxui::Color::GreenLight;
    }
    else if (message_id == message_menu_grade_edit::DELETE_FAILED)
    {
        result_message.msg = "😅 删除失败!";
        result_message.color = ftxui::Color::RedLight;
    }
    else if (message_id == message_menu_grade_edit::WRITE_SUCCESSED_SWITCH_NEXT)
    {
        result_message.msg = "🥳 输入成功!已智能为您切换到下一个学生!";
        result_message.color = ftxui::Color::GreenLight;
    }
    else if (message_id == message_menu_grade_edit::WRITE_SUCCESSED_LAST)
    {
        result_message.msg = "🥳 输入成功!已经是最后一个学生!";
        result_message.color = ftxui::Color::GreenLight;
    }
    else if (message_id == message_menu_grade_edit::WRITE_FAILED)
    {
        result_message.msg = "😅 写入失败!";
        result_message.color = ftxui::Color::RedLight;
    }

    return result_message;
}

Component App::menu_grade_get()
{
    // 存储是否仅自身lesson
    static bool only_self_lesson = true;

    // 存储教师id
    static std::string user_id = ""; // 教师id
    account.get_user_id(user_id);

    // 存储学生信息
    static std::vector<student_> result_student;

    // 科目下拉框
    static int lesson_select = 0;                   // 下拉框索引
    static std::vector<lesson> result_lesson;       // 查询lesson结果
    static std::vector<std::string> dropdown1_text; // 下拉框显示文本
    static auto dropdown1_option = dropdown_option_beautiful(&dropdown1_text, &lesson_select);
    dropdown1_option.radiobox.on_change = [&]() {
        if (lesson_select) // 选中非0,0是提示语
        {
            if (sql_db->get_course_student(result_lesson[lesson_select].class_id, result_student))
                result_student.erase(result_student.begin()); // 删除提示语
            else
                result_student.clear(); // 重置学生信息
        }
        else
            result_student.clear(); // 重置学生信息
    };
    static auto dropdown1 = Dropdown(dropdown1_option);

    // 初始化科目,初始化下拉框文本
    if (only_self_lesson)
        sql_db->get_teacher_lesson(user_id, result_lesson);
    else
        sql_db->get_teacher_lesson("%", result_lesson);
    for (auto &i : result_lesson)
        dropdown1_text.push_back(i.lesson_name);

    // 单选框,选择:按课程查询,按学生查询,按班级查询
    static int search_type = 0;
    static std::vector<std::string> radiobox_entries = {"按课程查询", "按学生查询", "按班级查询"};
    static auto radiobox_option = radiobox_option_beautiful();
    static auto radiobox1 = Radiobox(radiobox_entries, &search_type, radiobox_option);

    // 选择框_是否仅自身授课
    static auto checkbox_option = checkbox_option_beautiful();
    checkbox_option.on_change = [&]() {
        lesson_select = 0;
        dropdown1_text.clear(); // 重置科目下拉框
        if (only_self_lesson)
            sql_db->get_teacher_lesson(user_id, result_lesson);
        else
            sql_db->get_teacher_lesson("%", result_lesson);
        for (auto &i : result_lesson)
            dropdown1_text.push_back(i.lesson_name);
    };
    static auto checkbox1 = Checkbox("仅显示自身授课", &only_self_lesson, checkbox_option);

    auto childs = Container::Vertical({dropdown1, radiobox1, checkbox1});

    auto renderer = Renderer(childs, [&]() {
        // 创建课程总表,并且初始化信息
        std::string lesson_info;
        int student_num = 0;
        float grade_avg = 0.0f;
        int fail_num = 0;
        int pass_num = 0;
        int excellent_num = 0;
        lesson_info = result_lesson[lesson_select].lesson_name;
        student_num = result_student.size();
        float sum = 0.0f;
        for (auto &i : result_student)
        {
            grade temp_grade;
            if (!sql_db->get_student_course_grade(i.student_id, result_lesson[lesson_select].course_id, temp_grade))
                continue; // 读取失败,跳过
            sum += temp_grade.grade_total;
            if (temp_grade.grade_total < 60)
                fail_num++;
            else if (temp_grade.grade_total < 90)
                pass_num++;
            else
                excellent_num++;
        }
        if (student_num != 0)
            grade_avg = sum / student_num;

        // 创建授课成绩总表结构以及初始化表头数据
        std::vector<Elements> table1_elements = CreateTableHead_Lesson(lesson_info, student_num, grade_avg, fail_num, pass_num, excellent_num);
        //  创建学生成绩表结构以及初始化表头数据
        std::vector<Elements> table2_elements = CreateTableHead_Grade();

        // 插入学生成绩表数据
        for (auto &i : result_student)
        {
            grade temp_grade;
            if (!sql_db->get_student_course_grade(i.student_id, result_lesson[lesson_select].course_id, temp_grade))
                continue; // 读取失败,跳过
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
            table2_elements.push_back(temp_elements);
        }
        // 创建成绩总表,学生成绩表
        auto table1 = Table(table1_elements);
        auto document_table1 = table1.Render();
        auto table2 = Table(table2_elements);
        auto document_table2 = table2.Render();

        auto father_box = vbox({
            hbox({dropdown1->Render(), filler(), radiobox1->Render(), filler(), checkbox1->Render()}),
            separator(),
            hbox({document_table1 | center | border, document_table2 | center | border}),
        });
        father_box |= border;
        father_box |= size(WIDTH, EQUAL, ftxui::Terminal::Size().dimx);
        father_box |= size(HEIGHT, EQUAL, ftxui::Terminal::Size().dimy);
        father_box |= center;
        return father_box;
    });
    return renderer;
}