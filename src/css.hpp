#pragma once
#include "App.hpp"

auto dropdown_option_beautiful(std::vector<std::string> *radiobox_entries, int *radiobox_selected)
{
    auto dropdown_transform = [](bool open, Element checkbox, Element radiobox) {
        if (open)
            return vbox({checkbox | inverted, radiobox | vscroll_indicator | frame | size(HEIGHT, LESS_THAN, 5), filler()});
        else
            return vbox({checkbox, filler()});
    };
    auto dropdown_option = DropdownOption();
    dropdown_option.transform = dropdown_transform;
    dropdown_option.radiobox.entries = radiobox_entries;
    dropdown_option.radiobox.selected = radiobox_selected;
    return dropdown_option;
}

auto checkbox_option_beautiful()
{
    // clang-format off
    auto checkbox_option = CheckboxOption({
        .transform = [&](const EntryState &s) {
            auto prefix = text(s.state ? "✅ " : "🔳 "); // 选中,未选中的状态
            auto t = text(s.label);
            if (s.active)
                t |= bold;
            if (s.focused)
                t |= inverted;
            return hbox({prefix, t});
        }
    });
    // clang-format on
    return checkbox_option;
}

auto radiobox_option_beautiful()
{
    auto option = RadioboxOption();
    option.horizontal = true; // 水平显示
    option.transform = [](const EntryState &s) {
        auto prefix = text(s.state ? "✅ " : "🔳 ");
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
    return option;
}

std::vector<Elements> CreateTableHead_Grade()
{
    // 班级,学号姓名,平时分,期末分,总分
    std::vector<Elements> elements_arr;
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
    elements_arr.push_back(table_head);
    return elements_arr;
}

std::vector<Elements> CreateTableHead_Lesson(std::string lesson_info, int student_num, float grade_avg, int fail_num, int pass_num, int excellent_num)
{
    // 课程信息,学生数量,不及格人数(小于60分),及格人数(大于60分),优秀人数(大于90分),平均分,
    std::vector<Elements> elements_arr;
    elements_arr.push_back({text("📚 课  程"), separator(), text(lesson_info)});
    elements_arr.push_back({text("🙍🏻 人  数"), separator(), text(std::to_string(student_num))});
    elements_arr.push_back({text("😅 不及格 "), separator(), text(std::to_string(fail_num))});
    elements_arr.push_back({text("🤓 及  格 "), separator(), text(std::to_string(pass_num))});
    elements_arr.push_back({text("🤪 优  秀 "), separator(), text(std::to_string(excellent_num))});
    elements_arr.push_back({text("📊 平均分"), separator(), text(std::to_string(grade_avg))});

    return elements_arr;
}