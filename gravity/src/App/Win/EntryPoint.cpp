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
#include "../../Physics/GravitationalEnvironment.h"

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
    force11.setCoordinate(0, 0.0f);
    force11.setCoordinate(1, 0.0f);
    forceModel1.addVector(3, force11);
    DynamicCoordinatesImpl velocity1;
    velocity1.setCoordinate(0, 0.0f);
    velocity1.setCoordinate(1, 0.0f);
    DynamicPoint entity1(massPoint1, forceModel1, velocity1);

    // Entity 2
    DynamicCoordinatesImpl coordinates2;
    coordinates2.setCoordinate(1, 11.0f);
    coordinates2.setCoordinate(0, -19.0f);
    MassPoint massPoint2(&coordinates2, 1.0f);
    VectorModelImpl forceModel2(2);
    DynamicCoordinatesImpl force21;
    force21.setCoordinate(0, 0.0f);
    force21.setCoordinate(1, 0.0f);
    forceModel2.addVector(3, force21);
    DynamicCoordinatesImpl velocity2;
    velocity2.setCoordinate(0, 0.0f);
    velocity2.setCoordinate(1, 0.0f);
    DynamicPoint entity2(massPoint2, forceModel2, velocity2);

    // Entity 3
    DynamicCoordinatesImpl coordinates3;
    coordinates3.setCoordinate(1, 11.0f);
    coordinates3.setCoordinate(0, -18.0f);
    MassPoint massPoint3(&coordinates3, 1.0f);
    VectorModelImpl forceModel3(2);
    DynamicCoordinatesImpl force31;
    force31.setCoordinate(0, 0.0f);
    force31.setCoordinate(1, 0.0f);
    forceModel3.addVector(3, force31);
    DynamicCoordinatesImpl velocity3;
    velocity3.setCoordinate(0, 0.0f);
    velocity3.setCoordinate(1, 0.0f);
    DynamicPoint entity3(massPoint3, forceModel3, velocity3);

    simul.addEntity(10, entity1);

    GravitationalEnvironment grav(3.0f);

    // Gravity field 1
    GravityField gf1(massPoint1, true, force11);

    // Gravity field 2
    GravityField gf2(massPoint2, true, force21);

    // Gravity field 3
    GravityField gf3(massPoint3, true, force31);

    grav.addEntity(12, gf1);
    grav.addEntity(13, gf2);
    //grav.addEntity(10, gf3);
    
    int i = 0;
    for (;;) {
        grav.calculate();

        std::cout << "at " << 2.0f * i << " pos " << coordinates1.getCoordinate(0) << " " << 
            coordinates1.getCoordinate(1) << '\n';
        simul.simulate(0.1f);

        Sleep(1000);
        ++i;
    }

    disableConsole(program);
}
