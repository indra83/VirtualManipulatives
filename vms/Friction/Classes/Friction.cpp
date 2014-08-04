#include "Friction.h"
#include "BackGroundLayer.h"
#include "MenuLayer.h"
#include "SpriteLayer.h"

#define BG_ZINDEX 0
#define SP_ZINDEX 1
#define MN_ZINDEX 2

USING_NS_CC;

Scene* Friction::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::createWithPhysics();
    //scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    scene->getPhysicsWorld()->setGravity(Vect(0.0, -100.0));
    
    // 'layer' is an autorelease object
    auto layer = Friction::create();
    // add layer as a child to scene
    // attaches all the children to the existing physics world as well
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool Friction::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add the background layer
    auto bgLayer = BackGroundLayer::create();
    addChild(bgLayer, BG_ZINDEX); 

    /////////////////////////////
    // 3. add the sprite layer
    // add "HelloWorld" splash screen"
    auto spriteLayer = SpriteLayer::create();
    spriteLayer->setBackGroundLayer(bgLayer);
    addChild(spriteLayer, SP_ZINDEX);

    /////////////////////////////
    // 4. add the menu layer
    auto menuLayer = MenuLayer::create();
    menuLayer->setSpriteLayer(spriteLayer);
    addChild(menuLayer, MN_ZINDEX);
    return true;
}

