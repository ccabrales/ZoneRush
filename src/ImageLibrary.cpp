#include "ImageLibrary.h"



ImageLibrary::ImageLibrary(){
    
    
    
}

int ImageLibrary::size(){
    static ImageLibrary inst;
    return inst.images.size();
}

ofImage* ImageLibrary::getBullet(int imageNumber){
    static ImageLibrary inst;
    return inst.images[imageNumber];
};

