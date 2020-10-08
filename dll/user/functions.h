#include "il2cpp-appdata.h"
#include <helpers.h>
#include <vector>
#include <string>
#include <codecvt>
#include <iostream>
#include <Windows.h>

using namespace app;

void draw_header(HANDLE h_console);
bool is_in_game();
bool has_game_started();
std::vector<PlayerControl*> get_players();
std::string utf_from_net(String* net_string);
void clear_screen(int y);
std::string str_padding(std::string &s);