#include"window.hpp"
#ifdef FOR_WASM
#include"SDL2/SDL.h"
#include"SDL2/SDL_ttf.h"
#else
#include"SDL.h"
#include"SDL_ttf.h"
#endif
#include<iostream>
#include<vector>
#include<cstdlib>
#include<ctime>

#ifdef FOR_WASM
#include<emscripten/emscripten.h>
#endif


namespace game {

    // render object implementation for SDL2
    struct SDL_RenderObject : public RenderObject {
         SDL_Window *window = NULL;
         SDL_Renderer *ren = NULL;
         std::vector<SDL_Surface *> surfaces;
         std::vector<SDL_Texture *> textures;
         std::vector<TTF_Font *> fonts;
         std::vector<SDL_Joystick *> joysticks;
         SDL_Texture *tex = NULL;
         SDL_Surface *surface = NULL;
         int width = 0, height = 0;

        // destructor on clean up             
         ~SDL_RenderObject() {
            release_images();
            release_joysticks();
            std::cout << "game: SDL2 Shutdown...\n";
            TTF_Quit();
            SDL_Quit();
         }

        // release the joysticks if initalized
         void release_joysticks() {

            if(joysticks.size() == 0) {
                std::cout << "game: No joysticks to shutdown...\n";
            }

            for(std::vector<SDL_Joystick *>::size_type i = 0; i < joysticks.size(); ++i) {
                std::cout << "game:  Closing: " << SDL_JoystickName(joysticks[i]) << "\n";
                SDL_JoystickClose(joysticks[i]);
            }
         }

        // initalize the joysticks if available
         void init_joysticks() {

            for(int i = 0; i < SDL_NumJoysticks(); ++i) {
                SDL_Joystick *stick = SDL_JoystickOpen(i);
                if(stick != NULL) {
                    std::cout << "game: Openeed Joystick: " << SDL_JoystickName(stick) << "\n";
                    joysticks.push_back(stick);
                }
            }

            if(joysticks.size() == 0)
                std::cout << "game: No Joysticks found.\n";
                
         }

        // release image files
        void release_images() {
            for(std::vector<SDL_Surface*>::size_type i = 0; i < surfaces.size(); ++i) {
                SDL_FreeSurface(surfaces[i]);
                std::cout << "released image index: [" << i << "]\n";
            }
            if(!surfaces.empty())
                surfaces.erase(surfaces.begin(), surfaces.end());

            for(std::vector<TTF_Font *>::size_type i = 0; i< fonts.size(); ++i) {
                TTF_CloseFont(fonts[i]);
                std::cout << "released font index: [" << i << "]\n";
            }

            if(!fonts.empty()) 
                fonts.erase(fonts.begin(), fonts.end());

            for(std::vector<SDL_Texture *>::size_type i = 0; i < textures.size(); ++i) {
               SDL_DestroyTexture(textures[i]);
            }
            if(!textures.empty())
                textures.erase(textures.begin(), textures.end());
        }

        // release resources
        virtual void releaseResources() override {
            release_images();
        }

        // draw image at x,y
        void drawAt(Image image, int x, int y) override {
            SDL_Rect rc = { x, y, surfaces[image]->w, surfaces[image]->h };
            SDL_BlitSurface(surfaces.at(image), 0, surface, &rc);
        }

        // draw at rect x,y,w,h
        void drawAtRect(Image image, int x, int y, int w, int h) override {
            SDL_Rect rc = { x, y, w, h };
            SDL_BlitSurface(surfaces.at(image), 0, surface, &rc);
        }

        // draw at source rect dest rect
        void drawAtRect(Image image, int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2) override {
            SDL_Rect rc = { x1, y1, w1, h1 };
            SDL_Rect rc2 = { x2, y2, w2, h2 };
            SDL_BlitSurface(surfaces.at(image), &rc, surface, &rc2);
        }
        // print text with font
        void printText(Font font, int x, int y, const std::string &text, const Color &col) override {
            SDL_Color col_s = { col.r, col.g, col.b };
            SDL_Surface *surf = TTF_RenderText_Solid(fonts.at(font), text.c_str(), col_s);
            SDL_Rect rc = { x, y, surf->w, surf->h };
            SDL_BlitSurface(surf, 0, surface, &rc);
            SDL_FreeSurface(surf);
        }

        // draw At point
        void drawAt(Image image, const Point &p) override {
            drawAt(image, p.x, p.y);
        }
        // draw at rect
        void drawAtRect(Image image, const Rect &r) override {
            drawAtRect(image, r.x, r.y, r.w, r.h);
        }

        // draw at rect source rect, destination rect
        void drawAtRect(Image image, const Rect &src, const Rect &dst) override {
            drawAtRect(image, src.x, src.y, src.w, src.h, dst.x, dst.y, dst.w, dst.h);
        }

        // print text at Point with color
        void printText(Font font, const Point &p, const std::string &text, const Color &col) override {
            printText(font, p.x, p.y, text, col);
        }

       // load texture
       Texture loadTexture(const std::string &text) override {
            SDL_Surface *surf = SDL_LoadBMP(text.c_str());
            if(!surf) {
                std::cerr << "Error creating surface from: " << text << "\n";
                exit(EXIT_FAILURE);
            }
            SDL_Texture *t = SDL_CreateTextureFromSurface(ren, surf);
            if(!t) {
                std::cerr << "Error creating texture...\n";
                exit(EXIT_FAILURE);
            }
            textures.push_back(t);
            return textures.size()-1;
       }

      // return Image size
      virtual Size imageSize(Image img) override {
          if(img >= 0 && img < static_cast<int>(surfaces.size())) {
            return Size(surfaces[img]->w, surfaces[img]->h);
          }        
          return Size(0, 0);
      }


       // draw texture at rect
       void drawTextureAtRect(Texture tex, const Rect &r) override {
            SDL_Rect rc = { r.x, r.y, r.w, r.h };
            SDL_RenderCopy(ren, textures.at(tex), 0, &rc);
       }

        // load image file with default color key
        Image loadImage(const std::string &text) override {
           return loadImage(text, Color(255,255,255));
        }

        // load image file with color key
        Image loadImage(const std::string &text, const Color &color) override {
            SDL_Surface *surface = SDL_LoadBMP(text.c_str());
            if(!surface) {
                std::cerr << "Error could not load surface: " << text << "\n";
                exit(EXIT_FAILURE);
            }
            surfaces.push_back(surface);
            int index = surfaces.size()-1;
            setImageColorKey(index, color);
            std::cout << "loaded image [" << text << "] at index: [" << index << "]\n";
            return index;
        }

        // load image file from byte stream with color key
        Image loadImage(char *buf, int32_t size, const Color &c) override {
            SDL_RWops *ops = SDL_RWFromMem(buf, size);
            SDL_Surface *surface = SDL_LoadBMP_RW(ops, 1);
            if(!surface) {
                std::cerr << "Error could not load surface from memory \n";
                exit(EXIT_FAILURE);
            }
            surfaces.push_back(surface);
            int index = surfaces.size()-1;
            setImageColorKey(index, c);
            std::cout << "loaded image at index: [" << index << "]\n";
            return index;
        }


        // set Image color key
        void setImageColorKey(Image image, const Color &c) override {
            SDL_SetColorKey(surfaces[image], SDL_TRUE, SDL_MapRGB(surfaces[image]->format, c.r, c.g, c.b));
        }

        // load font
        Image loadFont(const std::string &text, int size) override {
            TTF_Font *font = TTF_OpenFont(text.c_str(), size);
            if(!font) {
                std::cerr << "Error could not load font: " << text << "\n" << SDL_GetError() << "\n";
                exit(EXIT_FAILURE);
            }
            fonts.push_back(font);
            int index = fonts.size()-1;
            std::cout << "loaded font [" << text << "] at index: [" << index << "]\n";
            return index;
        }

        void update() {

        }

        // initalize subsystem
        bool init(const std::string &text, int w, int h) {
            if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0) {
                std::cerr << "Error initaliziing SDL\n";
                return false;
            }
            srand(static_cast<unsigned int>(time(0))); 
            TTF_Init();

            window = SDL_CreateWindow(text.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, SDL_WINDOW_SHOWN);
            if(!window) {
                std::cerr << "Error creating window: " << SDL_GetError() << "...\n";
                return false;
            }
            ren = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
            if(!ren) {
                std::cerr << "Error creating Renderer: " << SDL_GetError() << "\n";
                return false;
            }

            tex = SDL_CreateTexture(ren, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, 1280-16, 720-16);

            if(!tex) {
                std::cerr << "Could not create texture: " << SDL_GetError() << "\n";
                return false;
            }

            surface = SDL_CreateRGBSurfaceFrom(NULL, 1280-16, 720-16, 32, w*4, 0x00FF0000, 0x0000FF00,0x000000FF,0xFF000000);

            if(!surface) {
                std::cerr << "Could not create main surface: " << SDL_GetError() << "\n";
                return false;
            }

            init_joysticks();

            width = w;
            height = h;
            return true;
        }

        // get system time ticks
        unsigned int getTicks() override {
            return static_cast<unsigned int>(SDL_GetTicks());
        }

        // is Key or joybutton pressed?
        bool keyDown(const Key &c) override {
            const Uint8 *keys = SDL_GetKeyboardState(0);
            switch(c) {
                case Key::KEY_LEFT:
                if(keys[SDL_SCANCODE_LEFT]) return true; 
                break;
                case Key::KEY_RIGHT:
                if(keys[SDL_SCANCODE_RIGHT]) return true;
                break;
                case Key::KEY_UP:
                if(keys[SDL_SCANCODE_UP]) return true;
                break;
                case Key::KEY_DOWN:
                if(keys[SDL_SCANCODE_DOWN]) return true;
                break;
                case Key::KEY_A:
                if(keys[SDL_SCANCODE_A]) return true;
                break;
                case Key::KEY_S:
                if(keys[SDL_SCANCODE_S]) return true;
                break;
                case Key::KEY_R:
                if(keys[SDL_SCANCODE_R]) return true;
                break;
            }

            for(std::vector<SDL_Joystick *>::size_type i = 0; i < joysticks.size(); ++i) {
                if(c == Key::KEY_A && SDL_JoystickGetButton(joysticks[i], 0)) {
                    return true;
                }
                if(c == Key::KEY_S && SDL_JoystickGetButton(joysticks[i], 1)) {
                    return true;
                }
                if(c == Key::KEY_LEFT && SDL_JoystickGetAxis(joysticks[i], 0) < -200) {
                    return true;
                }
                if(c == Key::KEY_RIGHT && SDL_JoystickGetAxis(joysticks[i], 0) > 200) {
                    return true;
                }
                if(c == Key::KEY_UP && SDL_JoystickGetAxis(joysticks[i], 1) > -200) {
                    return true;
                }
                if(c == Key::KEY_DOWN && SDL_JoystickGetAxis(joysticks[i], 1) > 200) {
                    return true;
                }
            }
            return false;
        }

    };

    bool Window::active;
    GameObject *Window::current_object = nullptr;
    SDL_RenderObject render_object;

    // create window
    bool Window::createWindow(const std::string &text, int w, int h) {
        return render_object.init(text, w, h);
    }
    // set GameObject 
    void Window::setObject(GameObject *obj) {
        Window::current_object = obj;
    }
    // intalize game object with render object
    void Window::init_object(GameObject *obj) {
        obj->init(&render_object);
    }

    void Window::eventProc() {
        static SDL_Event e;
        SDL_RenderClear(render_object.ren);          
        SDL_LockTexture(render_object.tex, 0, &render_object.surface->pixels, &render_object.surface->pitch);
        SDL_FillRect(render_object.surface, 0, 0);
        current_object->draw(&render_object);
        SDL_UnlockTexture(render_object.tex);
        SDL_Rect rc = { 0, 0, render_object.width, render_object.height };
        SDL_RenderCopy(render_object.ren, render_object.tex, 0, &rc);
        SDL_RenderPresent(render_object.ren);

        while(SDL_PollEvent(&e)) {
            switch(e.type) {
                case SDL_QUIT:
                active = false;
                break;
                case SDL_KEYDOWN:
                    if(e.key.keysym.sym == SDLK_ESCAPE)
                        active = false;
                    current_object->keydown(e.key.keysym.sym);
                break;
                case SDL_KEYUP:
                    current_object->keyup(e.key.keysym.sym);
                break;
            }
        }

    }

    // enter game loop
    int Window::loop() {

        if(current_object == nullptr) {
            std::cerr << "Error must initalize a game object.\n";
            return EXIT_FAILURE;
        }

        active = true;
        #ifndef FOR_WASM
        while (active) {
            eventProc();
        }
        #else
        emscripten_set_main_loop(eventProc, 0, 1);
        #endif
        current_object->release(&render_object);

        return EXIT_SUCCESS;
    }
}