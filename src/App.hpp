#pragma once
#include "ftxui/component/captured_mouse.hpp"
#include "ftxui/component/component.hpp"
#include "ftxui/component/component_base.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/elements.hpp"
#include <iostream>

#include "Account.hpp"
using namespace ftxui;
class App
{
  private:
    Account account;

  public:
    App();
    ~App();
    void login();
    void reg();
    void main_menu();
};
