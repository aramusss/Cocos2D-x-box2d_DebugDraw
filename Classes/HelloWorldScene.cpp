#include "HelloWorldScene.h"
#include "B2DebugDrawLayer.h"

USING_NS_CC;

#define PTM_RATIO 32

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
    auto label = Label::createWithTTF("Hello World", "fonts/Marker Felt.ttf", 24);
    
    // position the label on the center of the screen
    label->setPosition(Vec2(origin.x + visibleSize.width/2, origin.y + visibleSize.height - label->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(label, 1);
    
    ///////////////////////////////////////////////////////////
    CCLOG("Setting b2d world..");
    b2Vec2 gravity;
    gravity.Set(0, -10.f);
    world = new b2World(gravity);
    world->SetAllowSleeping(true);
    world->SetContinuousPhysics(true);
    debugDrawLayer = B2DebugDrawLayer::create(world, PTM_RATIO);
    debugDrawLayer->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2));
    this->addChild(debugDrawLayer, 9999);
    
    // Create edges around the entire screen
    
    b2Body *groundBody;
    b2BodyDef groundBodyDef;
    groundBody = world->CreateBody(&groundBodyDef);
    
    b2EdgeShape groundBox;
    b2FixtureDef groundBoxDef;
    groundBoxDef.shape = &groundBox;
    
    Vec2 boxPosition = Vec2(-300/PTM_RATIO, -300/PTM_RATIO);
    Vec2 boxVec2 = Vec2(200/PTM_RATIO, 200/PTM_RATIO);
    
    b2Fixture *bottomFixture;
    groundBox.Set(b2Vec2(boxPosition.x, boxPosition.y), b2Vec2(boxVec2.x, boxPosition.y));
    bottomFixture = groundBody->CreateFixture(&groundBoxDef);
    
    groundBox.Set(b2Vec2(boxPosition.x,boxPosition.y), b2Vec2(boxPosition.x, boxVec2.y));
    groundBody->CreateFixture(&groundBoxDef);
    
    groundBox.Set(b2Vec2(boxPosition.x, boxVec2.y), b2Vec2(boxVec2.x, boxVec2.y));
    groundBody->CreateFixture(&groundBoxDef);
    
    groundBox.Set(b2Vec2(boxVec2.x, boxVec2.y),
                  b2Vec2(boxVec2.x, boxPosition.y));
    groundBody->CreateFixture(&groundBoxDef);
    
    b2BodyDef ballBodyDef;
    ballBodyDef.type = b2_dynamicBody;
    ballBodyDef.position.Set(-200/PTM_RATIO, -200/PTM_RATIO);
    b2Body * ballBody = world->CreateBody(&ballBodyDef);
    
    // Create circle shape
    b2CircleShape circle;
    circle.m_radius = 20.0/PTM_RATIO;
    
    // Create shape definition and add to body
    b2FixtureDef ballShapeDef;
    ballShapeDef.shape = &circle;
    ballShapeDef.density = 1.0f;
    ballShapeDef.friction = 0.f;
    ballShapeDef.restitution = 1.0f;
    
    b2Fixture *ballFixture;
    ballFixture = ballBody->CreateFixture(&ballShapeDef);
    
    b2Vec2 force = b2Vec2(10, 10);
    b2Vec2 forcePoint = b2Vec2(-200/PTM_RATIO, -200/PTM_RATIO);
    ballBody->ApplyLinearImpulse(force, forcePoint, true);
    
    //world->DrawDebugData();
    
    this->schedule(schedule_selector(HelloWorld::tick), 0.001);
    return true;
}

void HelloWorld::tick(float dt)
{
    world->Step(dt, 10, 10);
}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
