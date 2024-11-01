#ifndef _GAME_H_PP
#define _GAME_H_PP
#include<string>
#include<algorithm>
#include<cstdint>
#define DEBUG_MODE 

namespace game {

    template<typename T>
    class TColor {
    public:
        T r, g, b;
        TColor();
        TColor(T r, T g, T b);
        TColor(const TColor<T> &c);
        TColor(TColor<T> &&c);
        TColor<T> &operator=(const TColor<T> &c);
        TColor<T> &operator=(TColor<T> &&c);
    };

    template<typename T>
    TColor<T>::TColor() : r{}, g{}, b{} {}

    template<typename T>
    TColor<T>::TColor(T rr, T gg, T bb) : r{rr}, g{gg}, b{bb} {}

    template<typename T>
    TColor<T>::TColor(const TColor<T> &c) : r{c.r}, g{c.g}, b{c.b} {}

    template<typename T>
    TColor<T>::TColor(TColor<T> &&c) : r{c.r}, g{c.g}, b{c.b} {}

    template<typename T>
    TColor<T> &TColor<T>::operator=(const TColor<T> &c) {
        r = c.r;
        g = c.g;
        b = c.b;
        return *this;
    }

    template<typename T>
    TColor<T> &TColor<T>::operator=(TColor<T> &&c) {
        r = c.r;
        g = c.g;
        b = c.b;
        return *this;
    }

    using Color = TColor<uint8_t>;

    template<typename T>
    class TRect {
    public:
        T x,y,w,h;
        TRect();
        TRect(T x, T y, T w, T h);
        TRect(const TRect<T> &r);
        TRect(TRect<T> &&r);
        TRect<T> &operator=(const TRect <T> &r);
        TRect<T> &operator=(TRect<T> &&r);
    };

    template<typename T>
    TRect<T>::TRect() : x{}, y{}, w{}, h{} {}

    template<typename T>
    TRect<T>::TRect(T xx, T yy, T ww, T hh) : x{xx}, y{yy}, w{ww}, h{hh} {}

    template<typename T>
    TRect<T>::TRect(const TRect<T> &r) : x{r.x}, y{r.y}, w{r.w}, h{r.h} {}

    template<typename T>
    TRect<T>::TRect(TRect<T> &&r) : x{r.x}, y{r.y}, w{r.w}, h{r.h} {}

    template<typename T>
    TRect<T> &TRect<T>::operator=(const TRect<T> &r) {
        x = r.x;
        y = r.y;
        w = r.w;
        h = r.h;
        return *this;
    }

    template<typename T>
    TRect<T> &TRect<T>::operator=(TRect<T> &&r) {
        x = r.x;
        y = r.y;
        w = r.w;
        h = r.h;
        return *this;
    }    

    using Rect = TRect<int32_t>;

    template<typename T>
    class TPoint {
    public:
        T x, y;
        TPoint();
        TPoint(const TPoint<T> &p);
        TPoint<T> &operator=(const TPoint<T> &p);
        TPoint(TPoint<T> &&p);
        TPoint<T> &operator=(TPoint<T> &&p);
        TPoint(T xx, T yy);
    };
   
    template<typename T>
    TPoint<T>::TPoint() : x{}, y{} {}

    template<typename T>
    TPoint<T>::TPoint(const TPoint<T> &p) : x{p.x}, y{p.y} {}

    template<typename T>
    TPoint<T> &TPoint<T>::operator=(const TPoint<T> &p) {
        x = p.x;
        y = p.y;
        return *this;
    }
    template<typename T>
    TPoint<T>::TPoint(TPoint<T> &&p) : x{p.x}, y{p.y} {}

    template<typename T>
    TPoint<T> &TPoint<T>::operator=(TPoint<T> &&p) {
        x = p.x;
        y = p.y;
        return *this;
    }

    template<typename T>
    TPoint<T>::TPoint(T xx, T yy) : x{xx}, y{yy} {}

    using Point = TPoint<int32_t>;
    using Size = TPoint<uint32_t>;

    enum class Key { KEY_LEFT=1, KEY_RIGHT, KEY_UP, KEY_DOWN, KEY_A, KEY_S, KEY_R };

    using Font = int;
    using Image = int;
    using Texture = int;

    struct RenderObject {
        virtual ~RenderObject() = default;
        virtual void drawAt(Image image, int x, int y) = 0;
        virtual void drawAt(Image image, const Point &p) = 0;
        virtual void drawAtRect(Image image, const Rect &r) = 0;
        virtual void drawTextureAtRect(Texture tex, const Rect &r) = 0;
        virtual void drawAtRect(Image image, const Rect &src, const Rect &dst) = 0;
        virtual void printText(Font font, const Point &p, const std::string &text, const Color &col) = 0;
        virtual void drawAtRect(Image image, int x, int y, int w, int h) = 0;
        virtual void drawAtRect(Image image, int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2) = 0;
        virtual void printText(Font font, int x, int y, const std::string &text, const Color &col) = 0;
        virtual Image loadImage(const std::string &text) = 0;
        virtual Image loadImage(const std::string &text, const Color &c) = 0;
        virtual Image loadImage(char *buf, int32_t size, const Color &c) = 0;
        virtual Size imageSize(Image img) = 0;
        virtual Texture loadTexture(const std::string &text) = 0;
        virtual void setImageColorKey(Image image, const Color &c) = 0;
        virtual Image loadFont(const std::string &text, int size) = 0;
        virtual unsigned int getTicks() = 0;
        virtual bool keyDown(const Key &c) = 0;
        virtual void releaseResources() = 0;
    };

    struct GameObject {
        virtual ~GameObject() = default;
        virtual void init(RenderObject *ro) = 0;
        virtual void draw(RenderObject *ro) = 0;
        virtual void release(RenderObject *ro) = 0;
        virtual void keydown(char key) = 0;
        virtual void keyup(char key) = 0;
    };
}

#endif