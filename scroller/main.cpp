#include"version.hpp"
#include"window.hpp"
#include"game_level.hpp"
#include<iostream>
#include"argz.hpp"


int main(int argc, char **argv) {

    std::string assets_path;

    std::cout << argv[0] << " v" << GAME_VERSION << " (C) 2024 LostSideDead Software\n";
    int width = 1280-16, height = 720-16;
    
#ifndef FOR_WASM
    Argz<std::string> argz(argc, argv);
    argz.addOptionSingleValue('r', "resolution ex 1280x720")
    .addOptionDoubleValue('R', "resolution", "resolution ex: 1280x720")
    .addOptionSingleValue('l', "level file")
    .addOptionSingleValue ('b',"background image")
    .addOptionSingleValue('g', "Graphics Package")
    .addOptionSingleValue('p', "path");

    std::string def_level, background, gfx_file;
    int value = 0;

    game::GameLevel level;
    

    Argument<std::string> arg;
    try {
        while((value = argz.proc(arg)) != -1) {
            switch(value) {
                case 'p':
                    assets_path = arg.arg_value;
                    std::cout << "Using path: " << assets_path << "\n";
                    level.assets_path = assets_path;
                break;
                case 'h':
                case 'v':
                    argz.help(std::cout);
                    exit(EXIT_SUCCESS);
                    break;
                case 'R':
                case 'r': {
                    auto pos = arg.arg_value.find("x");
                    if(pos == std::string::npos)  {
                        std::cerr << "use format WidthxHeight.\n";
                        std::cerr.flush();
                        exit(EXIT_FAILURE);
                    }
                    std::string left = arg.arg_value.substr(0, pos);
                    std::string right = arg.arg_value.substr(pos + 1);
                    width = atoi(left.c_str());
                    height = atoi(right.c_str());
                    std::cout << " Ssetting resolution: " << arg.arg_value << "\n";
                }
                break;
                case 'l':
                    def_level = assets_path + "/" + arg.arg_value;
                    break;
                case 'b':
                    background = assets_path + "/" + arg.arg_value;
                    break;
                case 'g':
                    gfx_file = assets_path + "/" + arg.arg_value;
                    break;
            }
        }
    } catch(const ArgException<std::string> &e) {
        std::cout << "Syntax Error: " << e.text() << "\n";
    }

    if(assets_path.empty()) {
        std::cerr<< "Game: requires assets path...\n";
        exit(EXIT_FAILURE);
    }

    if(gfx_file.empty() || background.empty() || def_level.empty()) {
        std::cout << "Game: falling back on defaults\n";
        
        def_level = assets_path + "/levels/level1.lvl";
        gfx_file = assets_path + "/img/level.gfx";
        background = assets_path + "/img/backgrounds/bg1.bmp";
    }
#else
    assets_path = "/assets";
#endif

    if(!game::Window::createWindow("Game", width, height)) {
        std::cerr << "Error on init.\n";
        return EXIT_FAILURE;
    }
    game::Window::init_object(&level);
    game::Window::setObject(&level);
    level.loadLevel(def_level,gfx_file,background);
    std::cout << "game: Level: " << def_level << " loaded.\n";
    fflush(stdout);
    return game::Window::loop();
}
