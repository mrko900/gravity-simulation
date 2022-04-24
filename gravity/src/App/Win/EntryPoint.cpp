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

#include "../../Physics/UniformForceSimulation.h"
#include "../../Physics/DynamicCoordinatesImpl.h"
#include "../../Physics/VectorModelImpl.h"

using namespace mrko900::gravity::physics;

int CALLBACK WinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPSTR lpCmdLine,
                     _In_ int nCmdShow) {
    Program program = newProgram();
    enableConsole(program);

    //WinProgram winProgram = WinProgram(hInstance, nCmdShow);
    //winProgram.run();

    UniformForceSimulation simul;

    // Entity 1
    DynamicCoordinatesImpl coordinates1;
    coordinates1.setCoordinate(1, 1.0f);
    coordinates1.setCoordinate(0, -1.0f);
    MassPoint massPoint1(&coordinates1, 1.0f);
    VectorModelImpl forceModel1(2);
    DynamicCoordinatesImpl force11;
    force11.setCoordinate(0, 1.0f);
    force11.setCoordinate(1, 0.0f);
    forceModel1.addVector(3, force11);
    DynamicCoordinatesImpl velocity1;
    velocity1.setCoordinate(0, -10.0f);
    velocity1.setCoordinate(1, 0.0f);
    DynamicPoint entity1(massPoint1, forceModel1, velocity1);

    simul.addEntity(10, entity1);
    
    int i = 0;
    for (;;) {
        std::cout << "at " << 2 * i << " pos " << coordinates1.getCoordinate(0) << " " << coordinates1.getCoordinate(1) << '\n';
        simul.simulate(2.0f);
        Sleep(1000);
        ++i;
    }

    disableConsole(program);
}
