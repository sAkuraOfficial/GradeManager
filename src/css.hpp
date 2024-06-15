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
            auto prefix = text(s.state ? "✅ " : "❎ "); // 选中,未选中的状态
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
