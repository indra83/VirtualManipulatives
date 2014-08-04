#ifndef __FRICTION_H__
#define __FRICTION_H__

#include "cocos2d.h"

class Friction : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
    
    // implement the "static create()" method manually
    CREATE_FUNC(Friction);
};

#endif // __FRICTION_H__
