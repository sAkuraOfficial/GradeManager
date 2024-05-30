#include "App.hpp"
App::App()
{
}

App::~App()
{
}

void App::login()
{
    auto screen = ScreenInteractive::TerminalOutput();

    std::string user_id, user_pwd;
    auto btn1 = Button(
        "登录", [&] {
            main_menu();
        }
    );
    auto btn2 = Button(
        "注册", [&] {
            reg();
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

void App::reg()
{
    auto screen = ScreenInteractive::TerminalOutput();

    std::string user_id, user_pwd;
    auto btn1 = Button(
        "确认注册", [&] {
            screen.Exit();
        }
    );
    auto btn2 = Button(
        "返回", [&] {
            screen.Exit();
        }
    );

    btn1 |= size(WIDTH, EQUAL, 15) | center;
    btn2 |= size(WIDTH, EQUAL, 15) | center;

    InputOption password_input_option;
    password_input_option.password = true;

    auto input1 = Input(&user_id, "请输入账号") | size(WIDTH, EQUAL, 20);
    auto input2 = Input(&user_pwd, "请输入密码", password_input_option) | size(WIDTH, EQUAL, 20);

    auto childs = Container::Vertical({input1, input2, btn1, btn2});

    auto component = Renderer(childs, [&] {
        auto child_box1 = vbox(
            {
                hbox({text("注册账号")}) | center,
                separator(),
                hbox({text("账号: "), input1->Render()}) | center,
                hbox({text("密码: "), input2->Render()}) | center,
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
                text("学生管理系统") | center | color(Color::Yellow),
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
