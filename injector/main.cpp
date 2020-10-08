#include <windows.h> 
#include <tlhelp32.h> 
#include <shlwapi.h> 
#include <conio.h> 
#include <stdio.h>
#include <thread>
#include <chrono>
#include <experimental/filesystem>
#include <iostream>

#define WIN32_LEAN_AND_MEAN

namespace fs = std::experimental::filesystem;

const char* ascii_logo =
"   ______           __    ___   __  __\n"
"  / ____/_  _______/ /_  /   \| / / / /\n"
" / /   / / / / ___/ __ \\/ /| |/ / / /\n"
"/ /___/ /_/ / /__/ / / / ___ / /_/ /\n"
"\\____/\\__, /\\___/_/ /_/_/  |_\\____/\n"
"     /____/ ";

void draw_header(HANDLE h_console) {
    SetConsoleTextAttribute(h_console, 15);
    std::cout << ascii_logo;
    SetConsoleTextAttribute(h_console, 11);
    std::cout << "A mini console cheat for Among Us. Made by Cych#0001." << std::endl;
    SetConsoleTextAttribute(h_console, 15);
    std::cout << "-----------------------------------------------" << std::endl 
        << "Note: This cheat is distributed for free on UnKnoWnCheaTs. If you purchased it somewhere, make a refund." << std::endl 
        << "Get it here: https://www.unknowncheats.me/forum/among-us/418478-cychau-mini-console-cheat.html" << std::endl << std::endl;
}

void print_error(HANDLE h_console, std::string text) {
    SetConsoleTextAttribute(h_console, 12);
    std::cout << text << std::endl;
    SetConsoleTextAttribute(h_console, 15);
}
void print_success(HANDLE h_console, std::string text) {
    SetConsoleTextAttribute(h_console, 11);
    std::cout << text << std::endl;
    SetConsoleTextAttribute(h_console, 15);
}

bool is_proc_running(const wchar_t* proc_name) {
    bool is_running = false;
    PROCESSENTRY32 process_entry;
    process_entry.dwSize = sizeof(PROCESSENTRY32);

    HANDLE proc_snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

    if (Process32First(proc_snap, &process_entry))
        while (Process32Next(proc_snap, &process_entry))
            if (!_wcsicmp(process_entry.szExeFile, proc_name))
                is_running = true;

    CloseHandle(proc_snap);
    return is_running;
}

int get_proc_id(const wchar_t* proc_name) {
    PROCESSENTRY32 process_entry;
    process_entry.dwSize = sizeof(PROCESSENTRY32);

    HANDLE proc_snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

    if (Process32First(proc_snap, &process_entry))
        while (Process32Next(proc_snap, &process_entry))
            if (!_wcsicmp(process_entry.szExeFile, proc_name)) {
                CloseHandle(proc_snap);
                return process_entry.th32ProcessID;
            }

    CloseHandle(proc_snap);
    return 0;
}

void inject_into_proc(std::string dll_name, int &process_id, HANDLE h_console) {
    dll_name = fs::absolute(dll_name).string();
    long dll_length = static_cast<long>(dll_name.length() + 1);
    HANDLE proc_handle = OpenProcess(PROCESS_ALL_ACCESS, false, process_id);
    if (proc_handle == NULL) {
        print_error(h_console, "[-] Failed opening process.");
        return;
    }
    LPVOID virt_alloc = VirtualAllocEx(proc_handle, NULL, dll_length, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
    if (virt_alloc == NULL) {
        print_error(h_console, "[-] Failed allocating memory inside of process.");
        return;
    }
    int write_dll_to_mem = WriteProcessMemory(proc_handle, virt_alloc, dll_name.c_str(), dll_length, 0);
    if (write_dll_to_mem == 0) {
        print_error(h_console, "[-] Failed writing DLL to memory.");
        return;
    }
    DWORD thread_id;
    LPTHREAD_START_ROUTINE load_lib = reinterpret_cast<LPTHREAD_START_ROUTINE>(GetProcAddress(LoadLibraryA("kernel32"), "LoadLibraryA"));
    HANDLE new_thread = CreateRemoteThread(proc_handle, NULL, 0, load_lib, virt_alloc, 0, &thread_id);
    if (new_thread == NULL) {
        print_error(h_console, "[-] Failed to create thread in process.");
        return;
    }
    if (proc_handle != NULL && virt_alloc != NULL && write_dll_to_mem != ERROR_INVALID_HANDLE && new_thread != NULL) {
        print_success(h_console, "[+] DLL injected successfully.");
    }
    return;
}

int main()
{
    HANDLE h_console = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTitle(L"CychAU Mini Console Injector");

    draw_header(h_console);

    if (fs::exists("./CychAU.dll")) {

        print_success(h_console, "[+] CychAU.dll found in the current directory.");

        std::cout << "Waiting for Among Us.exe..." << std::endl;
        while (!is_proc_running(L"Among Us.exe")) std::this_thread::sleep_for(std::chrono::milliseconds(200));
        print_success(h_console, "[+] Found Among Us.exe. Waiting a few seconds...");
        std::this_thread::sleep_for(std::chrono::milliseconds(5000));

        print_success(h_console, "[+] Attempting injection.");

        int proc_id = get_proc_id(L"Among Us.exe");
        if (!proc_id == 0) {
            inject_into_proc("./CychAU.dll", proc_id, h_console);
        }
        else {
            print_error(h_console, "[-] Something went wrong getting the PID.");
        }
    }
    else {
        print_error(h_console, "[-] CychAU.dll was not found in the current directory.");
    }
    return 0;
}