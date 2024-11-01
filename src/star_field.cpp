#include"star_field.hpp"

namespace game {
    // starfield constructor
    StarField::StarField() {
       
    }

    // initalize star field with number of total stars
    void StarField::init(int num_stars) {

        if(!stars.empty())
            stars.erase(stars.begin(), stars.end());

         for(int i = 0; i < num_stars; ++i) {
            stars.push_back({rand()%1280-20, rand()%720-20, rand()%static_cast<int>(images.size()), rand()%3});
        }
    }

    // set the star field images    
    void StarField::setImages(const std::vector<Image> &i) {
        if(!images.empty()) 
            images.erase(images.begin(), images.end());
        std::copy(i.begin(), i.end(), std::back_inserter(images));
    }

    // draw the star field
    void StarField::draw(RenderObject *ro) {
        if(images.size() == 0) return;
        for(auto i = stars.begin(); i != stars.end(); ++i) {
            ro->drawAt(images[i->type], i->x, i->y);            
        }
    }
    
    // process the positions of the stars
    void StarField::proc() {
        if(images.size() == 0) return;
        static int index = 0;
        ++index;

        if(index < 3) return;
        index = 0;        

        for(auto i = stars.begin(); i != stars.end(); ++i) {
            i->y ++;
            if(i->dir == 1) {
                i->x += 1;
            } else if(i->dir == 2) {
                i->x -= 1;
            }
            if(i->y > 720) {
                i->y = 0;
                i->x = rand()%1280;
                i->dir = rand()%3;
                i->type = rand()%2;
            }
        }
    }
 
}