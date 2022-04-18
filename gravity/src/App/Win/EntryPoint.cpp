#include <Windows.h>
#include <iostream>
#include "WinProgram.h"
#include "../ProgramLoop.h"

using mrko900::gravity::app::win::WinProgram;
using mrko900::gravity::app::ProgramLoop;

struct Program {
    FILE* fileConsole;
};

Program newProgram() {
    return Program { nullptr };
}

void enableConsole(Program& program) {
    AllocConsole();
    freopen_s(&program.fileConsole, "CONOUT$", "w", stdout);
    freopen_s(&program.fileConsole, "CONIN$", "r", stdin);
}

void disableConsole(const Program& program) {
    fclose(program.fileConsole);
    HWND consoleWindow = GetConsoleWindow();
    FreeConsole();
    PostMessageA(consoleWindow, WM_CLOSE, 0, 0);
}

int CALLBACK WinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPSTR lpCmdLine,
                     _In_ int nCmdShow) {
    Program program = newProgram();
    enableConsole(program);

    ProgramLoop programLoop;
    WinProgram winProgram = WinProgram(hInstance, nCmdShow, programLoop);
    winProgram.run();

    disableConsole(program);
}
