#include"camera.hpp"
#include"window_const.hpp"
namespace game {

    // default camera constructor
    Camera::Camera() : x{0}, y{0}, width{0}, height{0}, max_x{0}, max_y{0}, speed_x{0}, speed_y{0} {}

    // camera constructor
    Camera::Camera(int w, int h, int mx, int my) : x{0}, y{0}, width{w}, height{h}, max_x{mx}, max_y{my}, speed_x{WINDOW_SIZE_WIDTH}, speed_y{WINDOW_SIZE_HEIGHT} {}
   
    // copy constructor
    Camera::Camera(const Camera &c) : x{c.x}, y{c.y}, width{c.width}, height{c.height}, max_x{c.max_x}, max_y{c.max_y}, speed_x{c.speed_x}, speed_y{c.speed_y} {}

    // overloaded assignment operator
    Camera &Camera::operator=(const Camera &c) {
          set(c);
          return *this;
    }
    // set camera object
    void Camera::set(const Camera &c) {
        x = c.x;
        y = c.y;
        width = c.width;
        height = c.height;
        max_x = c.max_x;
        max_y = c.max_y;
        speed_x = c.speed_x;
        speed_y = c.speed_y;
    }

   // initalize camera
   void Camera::init(int w, int h, int mx, int my) {
        x = 0;
        y = 0;
        width = w;
        height = h;
        max_x = mx;
        max_y = my;
        speed_x = 16;
        speed_y = speed_x;// WINDOW_SIZE_HEIGHT;
   }

   // move camera
   void Camera::move(float delta, float dx, float dy) {
        float dx_val = dx * speed_x * delta;
        float dy_val = dy * speed_y * delta;
        x += static_cast<int>(dx_val);
        y += static_cast<int>(dy_val);
        x = std::max(0, std::min(x, max_x));
        y = std::max(0, std::min(y, max_y));
   }
   // get camera X,Y by cordinate
   void Camera::cameraXY(int hx, int hy, int &xx, int &yy) {
        int start_col = getX() / 16;
        int start_row = getY() / 16;
        int cx = getX();
        int cy = getY();
        int off_x = -cx + start_col * 16;
        int off_y = -cy + start_row * 16;
        xx = (hx - start_col) * 16 + off_x;
        yy = (hy - start_row) * 16 + off_y;       
   }

   // get X pos
   int Camera::getX() const { 
        return x; 
   }
   // get Y pos
   int Camera::getY() const {
        return y; 
   }
   // get camera X level coordinate
   int Camera::getCamX() const {
          return x/16;
   }
   // get camera Y level coordinate
   int Camera::getCamY() const {
          return y/16;
   }   

   // get width
   int Camera::getWidth() const { 
        return width; 
    }
   
   // get height
   int Camera::getHeight() const { 
        return height; 
    }

    // reset camera location to start position
    void Camera::reset() {
        x = y = 0;
    }
}