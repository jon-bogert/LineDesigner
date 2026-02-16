#include "App.h"

#include <XephTools/EntryPoint.h>

int Main(const std::vector<std::string>& args)
{
    App::Initialize();
    if (!args.empty() && std::filesystem::exists(args[0]))
    {
        App::TryOpen(args[0]);
    }
    App::Update();
    App::Shutdown();

    return 0;
}

SetEntryPointA(Main)