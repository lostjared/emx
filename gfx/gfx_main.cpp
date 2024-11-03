#ifdef _WIN32
#include"SDL.h"
#endif
#define VERSION_INFO "v0.1.0"
#include<iostream>
#include<string>
#include<sys/stat.h>
#include "gfx_file.hpp"
#include<unistd.h>

int main(int argc, char **argv) {
    int opt = 0;
    int mode = 0;
    std::string input, output;
    while((opt = getopt(argc, argv, "leci:o:hv")) != -1) {
        switch(opt) {
            case 'c':
                mode = 1;
                break;
            case 'e':
                mode = 2;
                break;
            case 'l':
                mode = 3;
                break;
            case 'i':
                input = optarg;
                break;
            case 'o':
                output = optarg;
                break;
            case 'h':
            case 'v':
                mode = 4;
                break;
        }
    }
    if(mode == 1 && input.length() > 0 && output.length() > 0) {
        game::GfxTable table;
        if(table.buildTable(input)) {
            game::GfxCompress comp;
            if(comp.open(output)) {
                std::cout << argv[0] << ": Compressing table: " << argv[1] << "\n";
                if(comp.compress(table)) {
                    comp.close();
                    std::cout << argv[0] << ": Success...\n";
                } else {
                    std::cout << argv[0] << ": Failure...\n";
                }
            }
        }
    } else if(mode == 2 && input.length() > 0 && output.length() > 0){
        game::GfxExtract ex;
        game::GfxTable table;
        if(ex.open(input)) {
#ifndef _WIN32
            mkdir(output.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
#else
	    mkdir(output.c_str());
#endif
            if(ex.extract(table, output)) {
                std::cout << argv[0] << ": Successflly Inflated: " << argv[1] << "\n";
            } else {
                std::cout << argv[0] << ": Failed to Inflate: " << argv[1] << "\n";
            }
        }
    } else if(mode == 3 && input.length() > 0) {
        game::GfxExtract ex;
        game::GfxTable table;
        if(ex.open(input)) {
            if(ex.list(table)) {
                std::cout << argv[0] << ": Successfully Listed: " << table.table.size() << " Items.\n";
            }
        }
    }
    else {
        std::cout << argv[0] << " " << VERSION_INFO << "\n";
        std::cout << "use:\n" << argv[0] << " [-c compress], [-e extract], [-l list] -i input -o output\n";
    }
    return 0;
}
