#include "functions.h"

using namespace app;

const char* ascii_logo =
"   ______           __    ___   __  __\n"
"  / ____/_  _______/ /_  /   \| / / / /\n"
" / /   / / / / ___/ __ \\/ /| |/ / / /\n"
"/ /___/ /_/ / /__/ / / / ___ / /_/ /\n"
"\\____/\\__, /\\___/_/ /_/_/  |_\\____/\n"
"     /____/ ";

void draw_header(HANDLE h_console) {
    SetConsoleTextAttribute(h_console, WHITE);
    std::cout << ascii_logo;
    SetConsoleTextAttribute(h_console, BLUE);
    std::cout << "A mini console cheat for Among Us. Made by Cych#0001." << std::endl;
    SetConsoleTextAttribute(h_console, WHITE);
    std::cout << "-----------------------------------------------" << std::endl 
        << "Note: This cheat is distributed for free on UnKnoWnCheaTs. If you purchased it somewhere, make a refund." << std::endl 
        << "Get it here: https://www.unknowncheats.me/forum/among-us/418478-cychau-mini-console-cheat.html" << std::endl << std::endl;
}

bool is_in_game() {
    return AmongUsClient__TypeInfo->static_fields->Instance->fields._.GameState == InnerNetClient_IFLBIJFJPMK__Enum_Joined
        || AmongUsClient__TypeInfo->static_fields->Instance->fields._.GameState == InnerNetClient_IFLBIJFJPMK__Enum_Started;
}

bool has_game_started() {
    return AmongUsClient__TypeInfo->static_fields->Instance->fields._.GameState == InnerNetClient_IFLBIJFJPMK__Enum_Started;
}

std::vector<PlayerControl*> get_players() {
    std::vector<PlayerControl*> players = std::vector<PlayerControl*>();
    List_1_PlayerControl_* player_list = PlayerControl__TypeInfo->static_fields->AllPlayerControls;

    for (int i = 0; i < List_1_PlayerControl__get_Count(player_list, NULL); i++) players.push_back(List_1_PlayerControl__get_Item(player_list, i, NULL));

    return players;
}


// Credits to IncoherentRage.
std::string utf_from_net(String* net_string) {
    std::wstring_convert<std::codecvt_utf8_utf16<char16_t, 0x10FFFF, std::codecvt_mode::little_endian>, char16_t> UTF16_TO_UTF8;

    if (net_string == NULL)
        return NULL;

    uint16_t* buffer = new uint16_t[net_string->fields.m_stringLength + 1];
    memcpy(buffer, &net_string->fields.m_firstChar, net_string->fields.m_stringLength * sizeof(uint16_t));
    buffer[net_string->fields.m_stringLength] = L'\0';
    std::string new_string = UTF16_TO_UTF8.to_bytes((const char16_t*)buffer);

    delete[] buffer;

    return new_string;
}

// Shamelessly ripped from https://stackoverflow.com/questions/10615042/how-to-update-refresh-console-screen
void clear_screen(int y)
{
    HANDLE                     hStdOut;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD                      count;
    DWORD                      cellCount;
    COORD                      homeCoords = { 0, y };

    hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hStdOut == INVALID_HANDLE_VALUE) return;

    /* Get the number of cells in the current buffer */
    if (!GetConsoleScreenBufferInfo(hStdOut, &csbi)) return;
    cellCount = csbi.dwSize.X * csbi.dwSize.Y;

    /* Fill the entire buffer with spaces */
    if (!FillConsoleOutputCharacter(
        hStdOut,
        (TCHAR)' ',
        cellCount,
        homeCoords,
        &count
    )) return;

    /* Fill the entire buffer with the current colors and attributes */
    if (!FillConsoleOutputAttribute(
        hStdOut,
        csbi.wAttributes,
        cellCount,
        homeCoords,
        &count
    )) return;

    /* Move the cursor home */
    SetConsoleCursorPosition(hStdOut, homeCoords);
}

std::string str_padding(std::string& s) {
    return std::string(11 - s.length(), ' ');
}