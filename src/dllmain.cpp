#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#pragma warning(push, 0)
#include <cocos2d.h>
#pragma warning(pop)
#include <MinHook.h>
#include <gd.h>

bool (__thiscall* GJGarageLayer_init)(cocos2d::CCLayer* self);

bool __fastcall GJGarageLayer_init_H(cocos2d::CCLayer* self, void*) {

    if (!GJGarageLayer_init(self)) return false;

    cocos2d::CCDictionary* statDict = gd::GameStatsManager::sharedState()->m_pPlayerStats;
    int demonCount = statDict->valueForKey("5")->intValue();

    auto demonLabel = cocos2d::CCLabelBMFont::create(std::to_string(demonCount).c_str(), "bigFont.fnt");

    auto winSize = cocos2d::CCDirector::sharedDirector()->getWinSize();

    int winSizeWidth = winSize.width;
    int winSizeHeight = winSize.height;

    demonLabel->setPosition(winSizeWidth - 36, winSizeHeight - 114);
    demonLabel->setAnchorPoint({ 1.f, 0.5f });
    demonLabel->setScale(0.45);

    cocos2d::CCSprite* demonSprite = cocos2d::CCSprite::createWithSpriteFrameName("diffIcon_06_btn_001.png");

    demonSprite->setPosition({ winSizeWidth - 20.f, winSizeHeight - 114.f });
    demonSprite->setAnchorPoint({ 0.5f, 0.5f });
    demonSprite->setScale(0.5);

    self->addChild(demonLabel);
    self->addChild(demonSprite);

    return true;
}


DWORD WINAPI thread_func(void* hModule) {
    MH_Initialize();


    auto base = reinterpret_cast<uintptr_t>(GetModuleHandle(0));
    MH_CreateHook(
        reinterpret_cast<void*>(base + 0x1255D0),
        GJGarageLayer_init_H,
        reinterpret_cast<void**>(&GJGarageLayer_init)
    );

    MH_EnableHook(MH_ALL_HOOKS);
    
    return 0;
}


BOOL APIENTRY DllMain(HMODULE handle, DWORD reason, LPVOID reserved) {
    if (reason == DLL_PROCESS_ATTACH) {
        CreateThread(0, 0x100, thread_func, handle, 0, 0);
        
    }
    return TRUE;
}