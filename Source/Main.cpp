#include "Sandbox.h"
#include <iostream> 

int main(int argc, char* argv[])
{
    Sandbox* sandbox = new Sandbox();
    if (!sandbox->Initialize())
    {
        std::cout << "Error with initialization." << std::endl;
        return 1;
    }

    while ( sandbox->IsRunning() )
    {
        sandbox->Loop();
    }

    sandbox->CleanUp();

    return 0;
}
