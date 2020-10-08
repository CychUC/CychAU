// Generated C++ file by Il2CppInspector - http://www.djkaty.com - https://github.com/djkaty
// Custom injected code entry point

#include "pch-il2cpp.h"

#define WIN32_LEAN_AND_MEAN

#include <iostream>
#include <thread>
#include <chrono>

#include "il2cpp-appdata.h"
#include "helpers.h"
#include "functions.h"

#include "magic_enum.h"

using namespace app;
// Set the name of your log file here
 extern const LPCWSTR LOG_FILE = L"CychAU.txt";

 bool show_imposters = false;

void Run()
{
	NewConsole();
	HANDLE h_console = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursor_info;

	GetConsoleCursorInfo(h_console, &cursor_info);
	cursor_info.bVisible = false;
	cursor_info.dwSize = 1;
	SetConsoleCursorInfo(h_console, &cursor_info);
	SetConsoleTitle(L"CychAU Mini Console Cheat");

	while (1) {
		draw_header(h_console);

		SetConsoleTextAttribute(h_console, RED);
		std::cout << "Waiting for game..." << std::endl;
		SetConsoleTextAttribute(h_console, WHITE);
		while (!is_in_game()) { std::this_thread::sleep_for(std::chrono::milliseconds(500)); }

		SetConsoleTextAttribute(h_console, BLUE);
		std::cout << "Game found. Waiting on game to start..." << std::endl;
		SetConsoleTextAttribute(h_console, WHITE);
		while (!has_game_started()) { std::this_thread::sleep_for(std::chrono::milliseconds(500)); }
		while (has_game_started()) {

			/*if (key_f1) {
				auto stat_manager_instance = GKIGKLGEHCF__TypeInfo->static_fields->GHJCLNEIJHD;
				if (stat_manager_instance != NULL) {
					stat_manager_instance->fields.banPoints = 0.0f;
					FindThisOnYourOwn:)(stat_manager_instance, NULL);
				}
			}*/

			if (GetAsyncKeyState(VK_F11) & 0x8000) {
				show_imposters = !show_imposters;
			}

			auto player_list = get_players();
			SetConsoleTextAttribute(h_console, YELLOW);
			std::cout << "Your Game:" << std::endl << "---------" << std::endl;
			SetConsoleTextAttribute(h_console, WHITE);
			for (auto player : player_list) {
				auto player_data = PlayerControl_get_Data(player, NULL);
				auto player_name = utf_from_net(player_data->fields.PlayerName);
				std::string padding = str_padding(player_name);

				std::string player_id = std::to_string(player_data->fields.PlayerId);
				std::string pet_id = std::to_string(player_data->fields.PetId);

				int color = (int)player_data->fields.ColorId;
				auto color_enum = magic_enum::enum_cast<player_colors>(color);
				auto color_str = magic_enum::enum_name(color_enum.value());
				std::string color_padding(6 - color_str.length(), ' ');

				TextRenderer* nameText = (TextRenderer*)(player->fields.RemainingEmergencies);

				List_1_GameData_CBOMPDNBEIF_* tasks = player_data->fields.Tasks;
				float compl_tasks = 0.0f;
				float incompl_tasks = 0.0f;
				float task_perc = 0.0f;

				if (List_1_GameData_CBOMPDNBEIF__get_Count(tasks, NULL) > 0) {
					for (int i = 0; i < List_1_GameData_CBOMPDNBEIF__get_Count(tasks, NULL); ++i) {
						GameData_CBOMPDNBEIF* task = List_1_GameData_CBOMPDNBEIF__get_Item(tasks, i, NULL);
						if (task->fields.Complete) compl_tasks += 1.0f;
						else incompl_tasks += 1.0f;
					}
					task_perc = (float)((int)(compl_tasks / (compl_tasks + incompl_tasks) * 100.f + .5f));
				}
				
				if (player_data->fields.IsImpostor && show_imposters)
					nameText->fields.Color = Palette__TypeInfo->static_fields->ImpostorRed;
				else if (player_data->fields.IsImpostor && !show_imposters)
					nameText->fields.Color = Palette__TypeInfo->static_fields->White;

				if (player_data->fields.IsDead && !player_data->fields.IsImpostor) {
					SetConsoleTextAttribute(h_console, PURPLE);
					std::cout << "[D] " << player_name << padding 
						<< "| PlayerID: " << player_id 
						<< " | Color: " << color_str << color_padding
						<< " | Task Completion: " << task_perc << "%"
						<< std::endl;
				}
				if (player_data->fields.IsDead && player_data->fields.IsImpostor) {
					SetConsoleTextAttribute(h_console, RED);
					std::cout << "[D] " << player_name << padding 
						<< "| PlayerID: " << player_id
						<< " | Color: " << color_str << color_padding
						<< " | Task Completion: " << "NULL"
						<< std::endl;
				}
				else if (!player_data->fields.IsDead && player_data->fields.IsImpostor) {
					SetConsoleTextAttribute(h_console, RED);
					std::cout << "[I] " << player_name << padding 
						<< "| PlayerID: " << player_id
						<< " | Color: " << color_str << color_padding
						<< " | Task Completion: " << "NULL"
						<< std::endl;
				}
				else if(!player_data->fields.IsDead && !player_data->fields.IsImpostor) {
					SetConsoleTextAttribute(h_console, WHITE);
					std::cout << "[C] " << player_name << padding 
						<< "| PlayerID: " << player_id
						<< " | Color: " << color_str << color_padding
						<< " | Task Completion: " << task_perc << "%"
						<< std::endl;
				}
				SetConsoleTextAttribute(h_console, WHITE);
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(2000));
			clear_screen(10);
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(5000));
		clear_screen(0);
	}
}