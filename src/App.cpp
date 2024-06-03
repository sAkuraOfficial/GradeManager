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
    auto screen = ScreenInteractive::TerminalOutput();

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

    InputOption password_input_option;
    password_input_option.password = true;

    auto input1 = Input(&user_id, "请输入账号") | size(WIDTH, EQUAL, 20);
    auto input2 = Input(&user_pwd, "请输入密码", password_input_option) | size(WIDTH, EQUAL, 20);

    auto childs = Container::Vertical({input1, input2, btn1, btn2, btn3});

    auto component = Renderer(childs, [&] {
        auto child_box1 = vbox(
            {
                hbox({text("欢迎使用学生成绩管理系统")}) | center,
                separator(),
                hbox({text("账号: "), input1->Render()}) | center,
                hbox({text("密码: "), input2->Render()}) | center,
                text(message),
                separator(),
                hbox({btn1->Render(), btn2->Render(), btn3->Render()}) | center,
            }
        );
        child_box1 |= border | size(WIDTH, EQUAL, 40) | center;

        auto father_box = vbox(
            {
                separatorEmpty(),
                separatorEmpty(),
                separatorEmpty(),
                separatorEmpty(),
                separatorEmpty(),
                separatorEmpty(),
                separatorEmpty(),
                separatorEmpty(),
                child_box1,
            }
        );
        return father_box;
    });

    screen.Loop(component);
}

void App::reg(bool &reg_result)
{
    auto screen = ScreenInteractive::TerminalOutput();

    bool user_type = false;

    std::string user_id, user_pwd, user_name, citizen_id;
    std::string reg_message = "请注册......";
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

    InputOption password_input_option;
    password_input_option.password = true;

    auto input1 = Input(&user_id, "请输入账号") | size(WIDTH, EQUAL, 20);
    auto input2 = Input(&user_pwd, "请输入密码", password_input_option) | size(WIDTH, EQUAL, 20);
    auto input3 = Input(&user_name, "请输入姓名") | size(WIDTH, EQUAL, 20);

    auto childs = Container::Vertical({input1, input2, input3, btn1, btn2});

    auto component = Renderer(childs, [&] {
        auto child_box1 = vbox(
            {
                hbox({text("注册账号")}) | center,
                separator(),
                hbox({text("账号: "), input1->Render()}) | center,
                hbox({text("密码: "), input2->Render()}) | center,
                hbox({text("姓名: "), input3->Render()}) | center,
                text(reg_message),
                separator(),
                hbox({btn1->Render(), btn2->Render()}) | center,
            }
        );
        child_box1 |= border | size(WIDTH, EQUAL, 40) | center;

        auto father_box = vbox(
            {
                separatorEmpty(),
                separatorEmpty(),
                separatorEmpty(),
                separatorEmpty(),
                separatorEmpty(),
                separatorEmpty(),
                separatorEmpty(),
                separatorEmpty(),
                child_box1,
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

    int menu_items_selected = 0;
    std::vector<std::string> menu_items = {
        "成绩录入",
        "成绩编辑",
        "班级管理",
        "数据看板",
        "导出数据",
        "退出系统"
    };

    auto main_menu = Menu(&menu_items, &menu_items_selected, MenuOption::HorizontalAnimated());

    auto main_menu_tab = Container::Tab(
        {
            // 页面
        },
        &menu_items_selected
    );

    // auto btn1 = Button("选择", [&] {});
    // auto btn2 = Button("退出", [&] {
    //     screen.Exit();
    // });

    // btn1 |= size(WIDTH, EQUAL, 15);
    // btn2 |= size(WIDTH, EQUAL, 15);

    auto childs = Container::Vertical({main_menu, main_menu_tab});

    auto component = Renderer(childs, [&] {
        auto father_box = vbox(
            {
                text("成绩管理系统") | center | color(Color::Yellow),
                separator(),
                vbox({main_menu->Render()}) | center,
                vbox({main_menu_tab->Render()}),
            }
        );

        return father_box|border;
        }
    );
    screen.Loop(component);
}
