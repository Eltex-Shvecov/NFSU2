#include <SFML/Graphics.hpp>
#include <windows.h>
#include <iostream>

#include "hook.h"
#include "nitro.h"
#include "engine.h"
#include "gear.h"

using namespace sf;

int main()
{
    Gear gear;
    Nitro nitro;
    Engine engine;
    NFSHook nfsHook;

    if (!nfsHook.HookInit())
    {
        std::cout << "HookInit error" << std::endl;
        return 1;
    }

    RenderWindow window(VideoMode(400, 100), "NFSU2 SFML");
    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
            {
                window.close();
            }
        }

        nfsHook.UpdateData();
        nitro.setValue(nfsHook.GetNitroValue());
        engine.set_value(nfsHook.GetEngineValue());
        gear.set_value(nfsHook.GetGearValue());
        window.clear(Color(130, 130, 130, 255));
        window.draw(nitro);
        window.draw(engine);
        window.draw(gear);
        window.display();
    }

    return 0;
}
