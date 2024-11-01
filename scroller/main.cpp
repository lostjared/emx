#include"version.hpp"
#include"window.hpp"
#include"game_level.hpp"
#include<iostream>


int main(int argc, char **argv) {

    std::string assets_path = "/assets";
    #ifndef FOR_WASM
    assets_path = "assets/";
    #endif

    std::cout << argv[0] << " v" << GAME_VERSION << " (C) 2023 LostSideDead Software\n";
    int width = 1280-16, height = 720-16;
    std::string def_level = assets_path + "/levels/level1.lvl";
    std::string gfx_file = assets_path + "/img/level.gfx";
    std::string background = assets_path + "/img/backgrounds/bg1.bmp";
    #ifndef FOR_WASM
    switch(argc) {
        case 4: {
            def_level = argv[1];
            gfx_file = argv[2];
            background = argv[3];
        }
            break;
        case 1:
        case 2:
        case 3: {
            std::cout << argv[0] << " level.lvl file.gfx background.bmp\n";
            return 0;
        } 
            break;
        case 6: {
            def_level = argv[1];
            gfx_file = argv[2];
            background = argv[3];
            width = atoi(argv[4]);
            height = atoi(argv[5]);
        } 
            break;
        default:
            std::cout << argv[0] << " level.lvl file.gfx background.bmp\n";
            break;
    }
    #endif
    if(!game::Window::createWindow("Game", width, height)) {
        std::cerr << "Error on init.\n";
        return EXIT_FAILURE;
    }
    game::GameLevel level;
    game::Window::init_object(&level);
    game::Window::setObject(&level);
    level.loadLevel(def_level,gfx_file,background);
    std::cout << "game: Level: " << def_level << " loaded.\n";
    fflush(stdout);
    return game::Window::loop();
}
