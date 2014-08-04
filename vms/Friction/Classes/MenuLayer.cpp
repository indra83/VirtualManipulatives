#include "MenuLayer.h"
#include "BackGroundLayer.h"
#include "SpriteLayer.h"

USING_NS_CC;
using namespace cocos2d::extension;

class ControlSliderRollBack : public ControlSlider
{
    float _base;
public :
    
    static ControlSliderRollBack* create(const char* bgFile, const char* progressFile, const char* thumbFile, float base)
    {
        ControlSliderRollBack *pRet = new ControlSliderRollBack();
        Sprite *backgroundSprite = Sprite::create(bgFile);
        Sprite *progressSprite = Sprite::create(progressFile);
        Sprite *thumbSprite = Sprite::create(thumbFile);

        pRet->initWithSprites(backgroundSprite, progressSprite, thumbSprite);
        pRet->_base = base;
        pRet->autorelease();
        return pRet;
    }

    void onTouchEnded(Touch *pTouch, Event *pEvent)
    {
        ControlSlider::onTouchEnded(pTouch, pEvent);
        setValue(_base);
    }

};

// on "init" you need to initialize your instance
bool MenuLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();

    //////////////////////////////
    // close button
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(MenuLayer::menuCloseCallback, this));

    closeItem->setPosition(Point(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
          origin.y + closeItem->getContentSize().height/2));

    //////////////////////////////
    // display controls
    float displayLocY = 3*visibleSize.height/8 + 40;
    float displayLocX = 3*visibleSize.width/4;
    float delta = 50;

#define CREATE_BUTTON(n, label, cb, enabled)\
    _item##n = MenuItemToggle::createWithCallback(CC_CALLBACK_1(cb, this),\
                                                      MenuItemFont::create( label" Disabled" ),\
                                                      MenuItemFont::create( label" Enabled" ), NULL );\
    _item##n->setPosition(Point(displayLocX, displayLocY));\
    _item##n->setSelectedIndex(enabled);\
    displayLocY -= delta;

    CREATE_BUTTON(1, "Show Forces", MenuLayer::menuCallbackShowForces, true)
    CREATE_BUTTON(2, "  Sum Of Forces", MenuLayer::menuCallbackShowSumOfForces, false)
    CREATE_BUTTON(3, "  Values", MenuLayer::menuCallbackShowValues, false)
    CREATE_BUTTON(4, "Masses", MenuLayer::menuCallbackShowMasses, false)
    CREATE_BUTTON(5, "Speed", MenuLayer::menuCallbackShowSpeed, false)

    auto menu = Menu::create(closeItem, _item1, _item2, _item3, _item4, _item5, NULL);
    menu->setPosition(Point::ZERO);
    addChild(menu);
 
    //////////////////////////////
    // controls on force
    //
    auto labelForce = LabelTTF::create("Change Force :", "fonts/Marker Felt.ttf", 40);
    labelForce->setPosition(Point(visibleSize.width/2, visibleSize.height/4 + 60));
    addChild(labelForce);

    auto slider = ControlSliderRollBack::create("sliderTrack.png", "sliderProgress.png" ,"sliderThumb.png", 0.0f);                        
    slider->setAnchorPoint(Point(0.5f, 0.5f));
    slider->setMinimumValue(-150.0f); // Sets the min value of range
    slider->setMaximumValue(150.0f); // Sets the max value of range
    slider->setPosition(Point(visibleSize.width/2, visibleSize.height/4));
    slider->setValue(0.0f);
    // When the value of the slider will change, the given selector will be called
    slider->addTargetWithActionForControlEvents(this, cccontrol_selector(MenuLayer::forceValueChanged), Control::EventType::VALUE_CHANGED);                        
    addChild(slider);
    auto labelL = LabelTTF::create("-150", "fonts/Marker Felt.ttf", 24);
    labelL->setPosition(Point(slider->getPosition().x - slider->getContentSize().width/2, slider->getPosition().y - 20));
    addChild(labelL);
    auto labelM = LabelTTF::create("0", "fonts/Marker Felt.ttf", 24);
    labelM->setPosition(Point(slider->getPosition().x, slider->getPosition().y - 20 ));
    addChild(labelM);
    auto labelH = LabelTTF::create("150", "fonts/Marker Felt.ttf", 24);
    labelH->setPosition(Point(slider->getPosition().x + slider->getContentSize().width/2, slider->getPosition().y - 20));
    addChild(labelH);

    // TODO: add a stepper for force
 
    //////////////////////////////
    // controls on friction
    auto labelF = LabelTTF::create("Change Friction :", "fonts/Marker Felt.ttf", 24);
    labelF->setPosition(Point(displayLocX, displayLocY));
    displayLocY -= 20;
    addChild(labelF);
    auto fSlider = ControlSlider::create("sliderTrack.png", "sliderProgress.png" ,"sliderThumb.png");                        
    fSlider->setAnchorPoint(Point(0.5f, 0.5f));
    fSlider->setMinimumValue(0.0f); // Sets the min value of range
    fSlider->setMaximumValue(1.0f); // Sets the max value of range
    fSlider->setPosition(Point(displayLocX, displayLocY));
    fSlider->setValue(0.5f);
    fSlider->setScale(0.75f);
    // When the value of the slider will change, the given selector will be called
    fSlider->addTargetWithActionForControlEvents(this, cccontrol_selector(MenuLayer::frictionValueChanged), Control::EventType::VALUE_CHANGED);                        
    addChild(fSlider);

    auto labelL1 = LabelTTF::create("Low", "fonts/Marker Felt.ttf", 24);
    labelL1->setPosition(Point(fSlider->getPosition().x - fSlider->getContentSize().width*fSlider->getScale()/2, fSlider->getPosition().y - 20));
    addChild(labelL1);
     auto labelH1 = LabelTTF::create("High", "fonts/Marker Felt.ttf", 24);
    labelH1->setPosition(Point(fSlider->getPosition().x + fSlider->getContentSize().width*fSlider->getScale()/2, fSlider->getPosition().y - 20));
    addChild(labelH1);
 
    //////////////////////////////
    // add label
    auto label = LabelTTF::create("Friction", "fonts/Marker Felt.ttf", 24);
    label->setPosition(Point(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));
    addChild(label);

    return true;
}
    
void MenuLayer::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();
}

void MenuLayer::menuCallbackShowSumOfForces(Ref* pSender)
{
    MenuItemToggle* pLabel = (MenuItemToggle *)pSender; 
    _spriteLayer->showSumOfForces(pLabel->getSelectedIndex());
}

void MenuLayer::menuCallbackShowValues(Ref* pSender)
{
    MenuItemToggle * pLabel = (MenuItemToggle *)pSender; 
    _spriteLayer->showForceValues(pLabel->getSelectedIndex());
}

void MenuLayer::menuCallbackShowForces(Ref* pSender)
{
    MenuItemToggle * pLabel = (MenuItemToggle *)pSender; 
    _spriteLayer->showForces(pLabel->getSelectedIndex());
    _item2->setEnabled(pLabel->getSelectedIndex());
    _item3->setEnabled(pLabel->getSelectedIndex()); 
    if (!pLabel->getSelectedIndex()) 
    {
        _item2->setSelectedIndex(0);
        _item3->setSelectedIndex(0);
    }
}

void MenuLayer::menuCallbackShowMasses(Ref* pSender)
{
    MenuItemToggle * pLabel = (MenuItemToggle *)pSender; 
    _spriteLayer->showMasses(pLabel->getSelectedIndex());
}

void MenuLayer::menuCallbackShowSpeed(Ref* pSender)
{
    MenuItemToggle * pLabel = (MenuItemToggle *)pSender; 
    _spriteLayer->showSpeeds(pLabel->getSelectedIndex());
}

void MenuLayer::forceValueChanged(Ref* sender, Control::EventType controlEvent)
{
    ControlSlider* pSlider = (ControlSlider*)sender;
    _spriteLayer->changeForceValue(pSlider->getValue());
}

void MenuLayer::frictionValueChanged(Ref* sender, Control::EventType controlEvent)
{
    ControlSlider* pSlider = (ControlSlider*)sender;
    _spriteLayer->changeFrictionValue(pSlider->getValue());
}

void MenuLayer::setSpriteLayer(SpriteLayer* layer) 
{ 
    CC_SAFE_RETAIN(layer); 
    _spriteLayer = layer; 
}

MenuLayer::~MenuLayer() 
{ 
}
