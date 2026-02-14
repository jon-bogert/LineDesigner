#include "App.h"

#include <XephTools/EntryPoint.h>

int Main(const std::vector<std::string>& args)
{
    App::Initialize();
    App::Update();
    App::Shutdown();

    return 0;
}

SetEntryPointA(Main)