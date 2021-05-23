#include <pthread.h>
#include <jni.h>
#include <memory.h>
#include <dlfcn.h>
#include <cstdio>
#include <cstdlib>
#include <Includes/RGBChanger.h>
#include <Includes/Vector3.h>
#include <Includes/Vector2.h>
#include <Includes/Quaternion.h>
#include <Includes/Rect.h>

#include "Includes/Logger.h"
#include "Patching/Patch.h"
#import "Includes/Utils.h"
#include "Includes/Color.h"
#include "Includes/Strings.h"
#include "Includes/Offsets.h"
#include "Includes/Chams.h"
#include "Includes/javaColor.h"
#include "Includes/UnityEngine.CoreModule.h"

#if defined(__armv7__)
#include "X64Hook/And64InlineHook.hpp"
#else
#include "Substrate/CydiaSubstrate.h"
#endif

bool chams = false;
bool isChatSpam = false;
bool isInvisible = false;
bool isVisible = false;
bool isGod = false;
bool isGod2 = false;
bool isSetBigHead = false;
bool isJetPack = false;
bool isMushroom = false;
bool isScaleUp = false;
bool isScaleDown = false;
bool isName = false;
bool isAddCoins = false;
bool isAddGems = false;
bool isAddCoupons = false;
bool isAddEventCurrency = false;
bool isAddBattlePassCurrency = false;

bool isRecoil = false;
bool isScatterReduction = false;
bool isCriticalHit = false;
bool isRegenHealth = false;
bool isRegenArmor = false;
bool isAmmo1 = false;
bool isAmmo2 = false;
bool isLevel = false;
bool isAutoShoot = false;
bool isVip = false;
bool isRapidfire = false;
bool isDamage = false;
bool killself = false;
bool suicideFall = false;
bool imSuicide = false;
bool suicideFallCoroutine = false;
bool autoShootDelay = false;
bool autoAimDistance = false;
bool radiusAutoAim = false;
bool effectiveDistance = false;
bool minDistanceAutoAim = false;
bool instantrespawn = false;
bool ninjaJump = false;
bool togglenoclip = false;
bool isGadgetModificator = false;
bool isRocketClip = false;
bool isReloadSpeed = false;

bool isBulletexplode = false;
bool isShotgun = false;
bool isReflectdamage = false;
bool isRrailgun = false;
bool isRocket = false;
bool isDamageReflectionValue = false;
bool isAimbot = false;
// bool isRange1 = false;
// bool isRange2 = false;
bool isDash = false;
bool isMoney = false;
// bool isMoney2 = false;
bool isCharhed = false;
bool noclip = false;
bool isFly = false;
bool isChatSpamClan = false;

float levelModifier = 1;
float setkillsModifier = 0;

struct Patches{
    Patch *Recoil;
    Patch *ScatterReduction;
    Patch *CriticalShot;
    Patch *RegenHealth;
    Patch *RegenArmor;
    Patch *Ammo1;
    Patch *Ammo2;
    Patch *Level;
    Patch *AutoShoot;
    Patch *Vip;
    Patch *Rapidfire;
    Patch *Damage;
    Patch* Killself;
    Patch* SuicideFall;
    Patch* ImSuicide;
    Patch* SuicideFallCoroutine;
    Patch* AutoShootDelay;
    Patch* AutoAimDistance;
    Patch* RadiusAutoAim;
    Patch* EffectiveDistance;
    Patch* MinDistanceAutoAim;
    Patch* Instantrespawn;
    Patch* NinjaJump;
    Patch* GadgetModificator;
    Patch* IsSkipCollision;
    Patch* ReloadSpeed;
}patch;

bool PlayerMoveCHookUpdateInitialized = false;
bool AccountBlockHook = false;
bool bankcontrollerhookinitialized = false;
bool weapsoundsHookInitialized = false;
bool FirstPersonControlSharpHookInitialized = false;
bool clanchatcontrollerhookinitialized = false;

void *(*Component_GetTransform)(void* component) = (void *(*)(void* ))getRealOffset(0x4072DD4); // Component$$get_transform
Vector3 (*Transform_get_localScale)(void* transform) = (Vector3 (*)(void *))getRealOffset(0x3E86340); // Transform$$get_localScale
void (*Transform_set_localScale)(void* transform, Vector3 pos) = (void (*)(void*, Vector3))getRealOffset(0x3E86408); // Transform$$set_localScale

void *MyPlayer;

/*
void* GetTransform(void* Player_move_c)
{

  return *(void**)((uint64_t)Player_move_c + 0x1C0); //myPlayerTransform
}
*/

bool get_isMine(void *Player){
  return *(bool *)((uint64_t)Player + 0x2D5); //Yes, "IsMine" is still in Player_move_c it just goes under a different from the other isMines because of the underscore at the beginning (_isMine)
}

// _isMine$$丅一丗丄世丄丒业七 ^^^

void MeScaleUp(void* myplayer) {
    Vector3 mysize = Transform_get_localScale(Component_GetTransform(myplayer));
    Vector3 mysize2 = mysize + Vector3(0.5,0.5,0.5);
    Transform_set_localScale(Component_GetTransform(myplayer), mysize2);
}

void MeScaleDown(void* myplayer) {
    Vector3 mysize = Transform_get_localScale(Component_GetTransform(myplayer));
    Vector3 mysize2 = mysize + Vector3(-0.5f, -0.5f, -0.5f);
    Transform_set_localScale(Component_GetTransform(myplayer), mysize2);
}

void (*ActivateXRay)(void* _this, bool xrayActive);
void (*SetXrayShader)(void* _this, bool active);
void (*SendChat)(void* _this, monoString* text, bool clanMode, monoString* iconName);
void (*setcurhealth)(void* _this, float amount);
void (*setcountkills)(void* _this, int amount);
void (*setisImmortality)(void* _this, bool value);
void (*SetBigHeadEffect)(void* _this, bool activate);
void (*ActivateJetpackGadget)(void* _this, bool isEnabled);

void(*old_PlayerMoveCUpdate)(void *instance);
void PlayerMoveCUpdate(void *instance) {
  if(instance != NULL) {
      {
          //*(int *) ((uint64_t) instance + 0x344) = 203219142; 一三东丒上丅丞丅丌/myPlayerID
      }
      if(!PlayerMoveCHookUpdateInitialized){
          PlayerMoveCHookUpdateInitialized = true;
          LOGI("GameManager_LateUpdate hooked");
      }
      if (chams) {
          *(bool *) ((uint64_t) instance + 0x3C4) = true; //丑丛东世丅丑丛丘三
          ActivateXRay(instance, true);
          SetXrayShader(instance, true);
      }
      if (isChatSpam) {
          SendChat(instance, CreateMonoString("Subscribe To Ioopen!"), false, CreateMonoString("Subscribe To Ioopen!"));
      }
      if (isInvisible) {
          *(bool *) ((uint64_t) instance + 0x2D2) = true; // 丛丞且丄丅丙业且丐
      }
      if (isVisible) {
          *(bool *) ((uint64_t) instance + 0x2CF) = false; // 业丁东丄丞与一丏上
      }
      if (isGod) {
          setcurhealth(instance, 1.0f); //In-Game sets it at 2
      }
      if (setkillsModifier) {
          setcountkills(instance, (int) setkillsModifier);
      }
      if (isGod2) {
          setisImmortality(instance, true);
      }
      if (isSetBigHead) {
          SetBigHeadEffect(instance, true);
      }
      if (isJetPack) {
          ActivateJetpackGadget(instance, true);
      }
      if (isMushroom) {
          *(float *) ((uint64_t) instance + 0x6E4) = 9999; //mushroomRadius$$东丗七与且业丈丗一
          *(float *) ((uint64_t) instance + 0x6E8) = 500; //mushroomBurnTime$$丂丌与丏丌万丙东且
          *(float *) ((uint64_t) instance + 0x6F0) = 200.0; //mushroomBurnDamage$$丛万丒下丙丌丐且丂
      }
  }
  old_PlayerMoveCUpdate(instance);
}

void (*setNickName)(void *_instance, monoString *value);
void *(*getplayer)();

void(*old_PlayerMove)(void *instance);
void PlayerMove(void *instance)
{
  if(instance){
      if(get_isMine(instance)){
          MyPlayer = instance;
      } else {
          if (isScaleUp) {
              MeScaleUp(instance);
          }
          if (isScaleDown) {
              MeScaleDown(instance);
          }
          if(isName) {
              setNickName(getplayer(), CreateMonoString("<color=blue>[Chewy]</color>"));
          }
      }
  }
  old_PlayerMove(instance);
}

/*
void(*old_AccountBlockedWindow)(void *instance);
void AccountBlockedWindow(void *instance) {
  if(instance != NULL) {
      if(!AccountBlockHook){
          AccountBlockHook = true;
          LOGI("GameManager_LateUpdate hooked");
          *(int *) ((uint64_t) instance + 0x38) = 60;
          *(long *) ((uint64_t) instance + 0x30) = 60;
      }
  }
  old_AccountBlockedWindow(instance);
}
*/

void(*old_BankControllerUpdate)(void *instance);
void BankControllerUpdate(void *instance) {
  if(instance != NULL) {
      if(!bankcontrollerhookinitialized) {
          bankcontrollerhookinitialized = true;
          LOGI("GameManager_LateUpdate hooked");
      }
      if (isAddCoins) {
          {
              void (*AddCoins)(void* _this, int, bool, int) =
              (void (*)(void* _this, int, bool, int))getRealOffset(0x19F4DD0); // BankController$$ 丒与丒业不丆丆丐业

              AddCoins(instance, 100, true, 1);
              isAddCoins = !AddCoins;
          }
      }
      if (isAddGems) {
          {
              void (*AddGems)(void* _this, int, bool, int) =
              (void (*)(void* _this, int, bool, int))getRealOffset(0x19F5014); // BankController$$ 与丙万不丂丌丗业万

              AddGems(instance, 100, true, 1);
              isAddGems = !AddGems;
          }
      }
      if (isAddCoupons) {
          {
              void (*AddCoupons)(void* _this, int, bool, int) =
              (void (*)(void* _this, int, bool, int))getRealOffset(0x19F729C); // BankController$$与万与万丕丌丕丆丂

              AddCoupons(instance, 100, true, 1);
              isAddCoupons = !AddCoupons;
          }
      }
      if (isAddEventCurrency) {
          {
              void (*AddEventCurrency)(void* _this, int, bool, int) =
              (void (*)(void* _this, int, bool, int))getRealOffset(0x19F533C); // BankController$$世七丁丛丙上且丂专

              AddEventCurrency(instance, 100, true, 1);
              isAddEventCurrency = !AddEventCurrency;
          }
      }
      if (isAddBattlePassCurrency) {
          {
              void (*AddBattlePassCurrency)(void* _this, int, bool, int) =
              (void (*)(void* _this, int, bool, int))getRealOffset(0x19F55A0); // BankController$$丙丏业丘与丙丟丄下

              AddBattlePassCurrency(instance, 100, true, 1);
              isAddBattlePassCurrency = !AddBattlePassCurrency;
          }

      }
  }
  old_BankControllerUpdate(instance);
}

void(*old_WeapSoundsUpdate)(void *instance);
void WeapSoundsUpdate(void *instance) {
  if(instance != NULL) {
      if(!weapsoundsHookInitialized){
          weapsoundsHookInitialized = true;
          LOGI("GameManager_LateUpdate hooked");
      }
      if (isBulletexplode) {
          *(bool *) ((uint64_t) instance + 0x111) = true; //bulletExplode
      }
      if (isShotgun) {
          *(bool *) ((uint64_t) instance + 0x112) = true; //isShotgun
      }
      if (isReflectdamage) {
          *(int *) ((uint64_t) instance + 0x239) = 50; //isDamageReflection
      }
      if (isRrailgun) {
          *(bool *) ((uint64_t) instance + 0x124) = true; //railgun
      }
      if (isRocket) {
          *(bool *) ((uint64_t) instance + 0xAF) = true; //bazooka
      }
      if (isDamageReflectionValue) {
          *(float *) ((uint64_t) instance + 0x23C) = 200.0; //damageReflectionValue
      }
      if (isAimbot) {
          *(bool *) ((uint64_t) instance + 0x270) = true; //flamethrower
          *(float *) ((uint64_t) instance + 0x428) = 12313312; //range
          *(float *) ((uint64_t) instance + 0xF4) = 14121242; //radiusRoundMelee
      }
      if (isDash) {
          *(bool *) ((uint64_t) instance + 0x1DC) = true; //isDash
      }
      if (isMoney) {
          *(float *) ((uint64_t) instance + 0x258) = 3123112; //coinDropChance
          *(bool *) ((uint64_t) instance + 0x254) = true; //isCoinDrop
      }
      if (isCharhed) {
          *(float *) ((uint64_t) instance + 0x144) = 0; //chargeTime
      }
  }
  old_WeapSoundsUpdate(instance);
}

//Credits To Jbro129 <3
void (*_Update)(void* _this);
void Update(void* _this)
{
  if (_this)
  {
      void* skinName = *(void **)((uint64_t)_this + 0x348); // internal SkinName mySkinName; // 0x320
      void* firstPersonControl = *(void **)((uint64_t)skinName + 0xE4); // public FirstPersonControlSharp firstPersonControl; // 0xD8
      void* characterController = *(void **)((uint64_t)firstPersonControl + 0x78); // internal CharacterController character; // 丕丕一一丙丌丂与万

      void (*CharacterController_set_radius)(void* character, float radius) = (void (*)(void *, float ))getRealOffset(0x50081C8); // CharacterController$$set_radius

      if (togglenoclip)
      {
          if (noclip)
          {
              CharacterController_set_radius(characterController, INFINITY);
          }
          else
          {
              CharacterController_set_radius(characterController, 0.35f);
          }
      }
      _Update(_this);
  }
}

void(*old_FirstPersonControlSharp)(void *instance);
void FirstPersonControlSharp(void *instance) {
  if(instance != NULL) {
      if(!FirstPersonControlSharpHookInitialized) {
          FirstPersonControlSharpHookInitialized = true;
          LOGI("GameManager_LateUpdate hooked");
      }
      if (isFly) {
          *(bool *) ((uint64_t) instance + 0x170) = true; //canJump$$一不丟丄一上丒丑七
          *(bool *) ((uint64_t) instance + 0x240) = false; //secondJumpEnabled$$丌丌三上不丂丄丅丕
          *(bool *) ((uint64_t) instance + 0xE0) = true; //ninjaJumpUsed$$三丟丞丑丛丘世下一
      }
  }
  old_FirstPersonControlSharp(instance);
}

void (*SendChatMessage)(void* _this, monoString* text, monoString* iconName);

void(*old_ClanChatControllerUpdate)(void *instance);
void ClanChatControllerUpdate(void *instance) {
  if(instance != NULL) {
      if(!clanchatcontrollerhookinitialized){
          clanchatcontrollerhookinitialized = true;
          LOGI("GameManager_LateUpdate hooked");
      }
      if (isChatSpamClan) {
          SendChatMessage(instance, CreateMonoString("Chewy was here"), CreateMonoString("Chewy was here"));
      }
  }
  old_ClanChatControllerUpdate(instance);
}

int (*old_Level)(void *instance);
int Level(void *instance) {
  if (instance != NULL) {
      if (levelModifier) {
          return (int) levelModifier;
      }
  }
  return old_Level(instance);
}

void nopcode(void*) {return;}
void nop(DWORD offset)
{
  octo_hook((void*)getRealOffset(offset), (void*)nopcode, nullptr);
}

// we will run our patches in a new thread so our while loop doesn't block process main thread
void* hack_thread(void*) {
  LOGI("I have been loaded. Mwuahahahaha");
  // loop until our target library is found
  do {
      sleep(1);
  } while (!isLibraryLoaded(libName));
  LOGI("I found the il2cpp lib. Address is: %p", (void*)findLibrary(libName));
  LOGI("Hooking GameManager_LateUpdate");

  ActivateXRay = (void (*)(void*, bool))getRealOffset(0x50836B4); //且丂丙丌上丕丛丞丙
  SetXrayShader = (void (*)(void*, bool))getRealOffset(0x50462B0); //丞丟丝世专丝上丌与
  SendChat = (void (*)(void*, monoString*, bool, monoString*))getRealOffset(0x5063818); //SendChat:Player_move_c
  SendChatMessage = (void (*)(void*, monoString*, monoString*))getRealOffset(0x2D5371C); //SendChatMessage:ClanChatController
  setcountkills = (void (*)(void*, int))getRealOffset(0x504DAC8); //上丙世不丙丕丌丅与
  setcurhealth = (void (*)(void*, float))getRealOffset(0x50B3B98); //丂丏丛不三且丟丒丌
  setisImmortality = (void (*)(void*, bool))getRealOffset(0x504DC1C); //丕下不丌东丂三不丅
  SetBigHeadEffect = (void (*)(void*, bool))getRealOffset(0x50885C0); //丂且丈丗丅丆丗丁丘
  ActivateJetpackGadget = (void (*)(void*, bool))getRealOffset(0x506740C); //丘不丌丅丌丄丝三丆
  getplayer = (void *(*)())getRealOffset(0x258E328); //丒与七丑丐丌上丟下
  setNickName = (void(*)(void*, monoString*))getRealOffset(0x1BB47F0); //丟丟丂丑丙上丙丂万

  octo_hook((void*)getRealOffset(0x5091F28), (void*)PlayerMoveCUpdate, (void**)&old_PlayerMoveCUpdate);
  octo_hook((void*)getRealOffset(0x5082244), (void*)PlayerMove, (void**)&old_PlayerMove); //东且丕丕丕丕丈七世$$UpdateSkin
  // octo_hook((void*)getRealOffset(0x271A538), (void*)AccountBlockedWindow, (void**)&old_AccountBlockedWindow);
  octo_hook((void*)getRealOffset(0x19EFFBC), (void*)BankControllerUpdate, (void**)&old_BankControllerUpdate);
  octo_hook((void*)getRealOffset(0x1121CD8), (void*)WeapSoundsUpdate, (void**)&old_WeapSoundsUpdate);
  octo_hook((void*)getRealOffset(0x509AFD8), (void*)&Update, (void **)&_Update); // Player_move_c$$三丁丙丏三丕东丁东$$UpdateEffects
  octo_hook((void*)getRealOffset(0xF10154), (void*)FirstPersonControlSharp, (void **)&old_FirstPersonControlSharp);
  octo_hook((void*)getRealOffset(0x1F2FAC0), (void*)ClanChatControllerUpdate, (void **)&old_ClanChatControllerUpdate);
  octo_hook((void*)getRealOffset(0x141DA80), (void*)Level, (void**)&old_Level);//ExperienceController&&三丛且七丕上丂丁丅

  patch.Recoil = Patch::Setup((void*)getAbsoluteAddress(0x1F657B8), (char*)"\x01\x00\xa0\xe3\x1e\xff\x2f\xe1", 8); //丅三丑一丂丈丕丁丅
  patch.ScatterReduction = Patch::Setup((void*)getAbsoluteAddress(0x1F64FC8), (char*)"\x01\x00\xa0\xe3\x1e\xff\x2f\xe1", 8); //丂丛丐专丛下丐丏丅
  patch.CriticalShot = Patch::Setup((void*)getAbsoluteAddress(0x1F65218), (char*)"\x01\x00\xa0\xe3\x1e\xff\x2f\xe1", 8); //丕下业与丁丐丛下东
  patch.RegenHealth = Patch::Setup((void*)getAbsoluteAddress(0x1F5EFD4), (char*)"\x01\x00\xa0\xe3\x1e\xff\x2f\xe1", 8); //专丅万专不丙丒丟与
  patch.RegenArmor = Patch::Setup((void*)getAbsoluteAddress(0x1F5EF48), (char*)"\x01\x00\xa0\xe3\x1e\xff\x2f\xe1", 8); //丞丄丂上三丁不丗丏
  patch.Ammo1 = Patch::Setup((void*)getAbsoluteAddress(0x103A6E0), (char*)"\x64\x00\xa0\xe3\x1e\xff\x2f\xe1", 8); //get_currentAmmoInBackpack | 丆丞东丂上与丏不丟
  patch.Ammo2 = Patch::Setup((void*)getAbsoluteAddress(0x103A73C), (char*)"\x64\x00\xa0\xe3\x1e\xff\x2f\xe1", 8); //get_currentAmmoInClip | 不业且丘三丗丆丛丗
  patch.Level = Patch::Setup((void*)getAbsoluteAddress(0xF76AA4), (char*)"\x00\x00\xA0\xE3\x1E\xFF\x2F\xE1", 8); // 三丛且七丕上丂丁丅
  patch.AutoShoot = Patch::Setup((void*)getAbsoluteAddress(0x50D154C), (char*)"\x01\x00\xa0\xe3\x1e\xff\x2f\xe1", 8); // 丄世丞丗万丒丕丑专
  patch.Vip = Patch::Setup((void*)getAbsoluteAddress(0x204B2FC), (char*)"\x01\x00\xa0\xe3\x1e\xff\x2f\xe1", 8); // 丙世丘丙丕丒上世上
  patch.Rapidfire = Patch::Setup((void*)getAbsoluteAddress(0xD8DAA0), (char*)"\xFA\x04\x44\xE3\x1E\xFF\x2F\xE1", 8); // 上丄丈东丁与七丅专
  patch.Damage = Patch::Setup((void*)getAbsoluteAddress(0x50B4FC4), (char*)"\xFA\x04\x44\xE3\x1E\xFF\x2F\xE1", 8); // 不丟不丂东下万丙不
  patch.Killself = Patch::Setup((void*)getAbsoluteAddress(0x50C253C), (char*)"\x00\xF0\x20\xE3\x1E\xFF\x2F\xE1", 8); //丄七丌丑丂丟丌业业
  patch.SuicideFall = Patch::Setup((void*)getAbsoluteAddress(0x50C22A0), (char*)"\x00\xF0\x20\xE3\x1E\xFF\x2F\xE1", 8); //丝丁丂万丕丏丘丏不
  patch.ImSuicide = Patch::Setup((void*)getAbsoluteAddress(0x50B4394), (char*)"\x00\xF0\x20\xE3\x1E\xFF\x2F\xE1", 8); //丗东世丐丟专东丙丕
  patch.SuicideFallCoroutine = Patch::Setup((void*)getAbsoluteAddress(0x50C24B4), (char*)"\x00\xF0\x20\xE3\x1E\xFF\x2F\xE1", 8); //丛丘东丂丗与一丗专
  patch.AutoShootDelay = Patch::Setup((void*)getAbsoluteAddress(0x111CA44), (char*)"\x7A\x04\x44\xE3\x1E\xFF\x2F\xE1", 8); //丑七丑丗丅且世东丐
  patch.AutoAimDistance = Patch::Setup((void*)getAbsoluteAddress(0x111C5D4), (char*)"\x7A\x04\x44\xE3\x1E\xFF\x2F\xE1", 8); //get_AutoAimDistance
  patch.RadiusAutoAim = Patch::Setup((void*)getAbsoluteAddress(0x111C9A8), (char*)"\x7A\x04\x44\xE3\x1E\xFF\x2F\xE1", 8); //get_RadiusAutoAim
  patch.EffectiveDistance = Patch::Setup((void*)getAbsoluteAddress(0x111C90C), (char*)"\x7A\x04\x44\xE3\x1E\xFF\x2F\xE1", 8); //get_EffectiveDistance
  patch.MinDistanceAutoAim = Patch::Setup((void*)getAbsoluteAddress(0x111C74C), (char*)"\x7A\x04\x44\xE3\x1E\xFF\x2F\xE1", 8); //get_MinDistanceAutoAim
  patch.Instantrespawn = Patch::Setup((void*)getAbsoluteAddress(0x1F656E4), (char*)"\x01\x00\xa0\xe3\x1e\xff\x2f\xe1", 8); // 丈丟一业丙丈丟丆一
  patch.NinjaJump = Patch::Setup((void*)getAbsoluteAddress(0x1F5BA44), (char*)"\x01\x00\xa0\xe3\x1e\xff\x2f\xe1", 8); // 丁不丝丌丅丄丂专丒
  patch.GadgetModificator = Patch::Setup((void*)getAbsoluteAddress(0x50A7810), (char*)"\x01\x00\xA0\xE3\x1E\xFF\x2F\xE1", 8); //丄与丄不万丕七丕丗
  patch.IsSkipCollision = Patch::Setup((void*)getAbsoluteAddress(0xFBD1F4), (char*)"\x01\x00\xA0\xE3\x1E\xFF\x2F\xE1", 8); //丆丟丕东丛丌丟丘丂
  patch.ReloadSpeed = Patch::Setup((void*)getAbsoluteAddress(0xD8D8F4), (char*)"\xFA\x04\x44\xE3\x1E\xFF\x2F\xE1", 8); // 丂丝丝丘业丅丂业丑
    /*
    Patch::Setup((void*)getAbsoluteAddress(0x271AA28), (char*)"\x01\xF0\x20\xe3\x1e\xff\x2f\xe1", 8)->Apply();
    Patch::Setup((void*)getAbsoluteAddress(0x18316D0), (char*)"\x00\xF0\x20\xe3\x1e\xff\x2f\xe1", 8)->Apply();
    Patch::Setup((void*)getAbsoluteAddress(0x347DFC0), (char*)"\x01\xF0\x20\xe3\x1e\xff\x2f\xe1", 8)->Apply();
    Patch::Setup((void*)getAbsoluteAddress(0x2594074), (char*)"\x00\xF0\x20\xe3\x1e\xff\x2f\xe1", 8)->Apply();
    Patch::Setup((void*)getAbsoluteAddress(0x258DC18), (char*)"\x01\xF0\x20\xe3\x1e\xff\x2f\xe1", 8)->Apply();
    Patch::Setup((void*)getAbsoluteAddress(0x1341D70), (char*)"\x01\xF0\x20\xe3\x1e\xff\x2f\xe1", 8)->Apply();
    Patch::Setup((void*)getAbsoluteAddress(0x271A218), (char*)"\x01\xF0\x20\xe3\x1e\xff\x2f\xe1", 8)->Apply();
    Patch::Setup((void*)getAbsoluteAddress(0x271A260), (char*)"\x00\xF0\x20\xe3\x1e\xff\x2f\xe1", 8)->Apply();
    Patch::Setup((void*)getAbsoluteAddress(0x271A238), (char*)"\x01\xF0\x20\xe3\x1e\xff\x2f\xe1", 8)->Apply();
    */

    nop(0x1343BD8); //专丈丆业业世上丆丝$$OpLeaveLobby
    nop(0x1344FC4); //丑下与丅丅不业丗丐$$OpLeaveRoom
    return NULL;;
}


  jobjectArray getListFT(JNIEnv *env, jclass jobj){
    jobjectArray ret;
    int i;
    const char *features[]= {
            "Chams",
            "Chat Spam",
            "Invisible",
            "GodMode",
            "GodModeV2",
            "Big Head",
            "Add Coins",
            "Add Gems",
            "Add Coupons",
            "Add Event",
            "Add Battle Pass",
            "No Recoil",
            "No Spread",
            "Critical Hit",
            "Health Regeneration",
            "Armor Regeneration",
            "Inf Ammo",
            "Don't Level Up",
            "Auto Shoot",
            "Vip",
            "Firerate",
            "One Hit Kill",
            "Anti-Self Kill",
            "Increased Aim Assist",
            "Instant Respawn",
            "Double Jump",
            "Shoot exclusives",
            "Shoot Shotgun Bullets",
            "Thorns",
            "Shoot Laser Bullet",
            "Shoot Rockets",
            "Silent AimBot",
            "Shoot Dash",
            "Enemies Drop Coins When You Kill Them",
            "No Charge Time",
            "JetPack",
            "Op Mushroom",
            "Clan Chat Spam",
            "Player Scale Up",
            "Player Scale Down",
            "Name Changer",
            "NoClip",
            "Gadget Spam",
            "Rocket No Clip",
            "Fly",
            "Fast Reload",
            "SeekBar_Level Modifier_0_65",
            "SeekBar_Kills Modifier_0_999"
    };
    int Total_Feature = (sizeof features / sizeof features[0]); //Now you dont have to manually update the number everytime
    ret= (jobjectArray)env->NewObjectArray(Total_Feature,
                                           env->FindClass("java/lang/String"),
                                           env->NewStringUTF(""));

    for(i=0;i<Total_Feature;i++) {
        env->SetObjectArrayElement(
                ret,i,env->NewStringUTF(features[i]));
    }
    return(ret);
  }


  void changeToggle(JNIEnv *env, jclass thisObj, jint number) {
    int i = (int) number;
    switch (i) {
        case 0:
            chams = !chams;
            break;
        case 1:
            isChatSpam = !isChatSpam;
            break;
        case 2:
            isInvisible = !isInvisible;
            isVisible = !isVisible;
            break;
        case 3:
            isGod = !isGod;
            break;
        case 4:
            isGod2 = !isGod2;
            break;
        case 5:
            isSetBigHead = !isSetBigHead;
            break;
        case 6:
            isAddCoins = !isAddCoins;
            break;
        case 7:
            isAddGems = !isAddGems;
            break;
        case 8:
            isAddCoupons = !isAddCoupons;
            break;
        case 9:
            isAddEventCurrency = !isAddEventCurrency;
            break;
        case 10:
            isAddBattlePassCurrency = !isAddBattlePassCurrency;
            break;
        case 11:
            isRecoil = !isRecoil;
            if (isRecoil) {
                patch.Recoil->Apply();
            } else {
                patch.Recoil->Reset();
            }
            break;
        case 12:
            isScatterReduction = !isScatterReduction;
            if (isScatterReduction) {
                patch.ScatterReduction->Apply();
            } else {
                patch.ScatterReduction->Reset();
            }
            break;
        case 13:
            isCriticalHit = !isCriticalHit;
            if (isCriticalHit) {
                patch.CriticalShot->Apply();
            } else {
                patch.CriticalShot->Reset();
            }
            break;
        case 14:
            isRegenHealth = !isRegenHealth;
            if (isRegenHealth) {
                patch.RegenHealth->Apply();
            } else {
                patch.RegenHealth->Reset();
            }
            break;
        case 15:
            isRegenArmor = !isRegenArmor;
            if (isRegenArmor) {
                patch.RegenArmor->Apply();
            } else {
                patch.RegenArmor->Reset();
            }
            break;
        case 16:
            isAmmo1 = !isAmmo1;
            if (isAmmo1) {
                patch.Ammo1->Apply();
            } else {
                patch.Ammo1->Reset();
            }
            isAmmo2 = !isAmmo2;
            if (isAmmo2) {
                patch.Ammo2->Apply();
            } else {
                patch.Ammo2->Reset();
            }
            break;
        case 17:
            isLevel = !isLevel;
            if (isLevel) {
                patch.Level->Apply();
            } else {
                patch.Level->Reset();
            }
            break;
        case 18:
            isAutoShoot = !isAutoShoot;
            if (isAutoShoot) {
                patch.AutoShoot->Apply();
            } else {
                patch.AutoShoot->Reset();
            }
            break;
        case 19:
            isVip = !isVip;
            if (isVip) {
                patch.Vip->Apply();
            } else {
                patch.Vip->Reset();
            }
            break;
        case 20:
            isRapidfire = !isRapidfire;
            if (isRapidfire) {
                patch.Rapidfire->Apply();
            } else {
                patch.Rapidfire->Reset();
            }
            break;
        case 21:
            isDamage = !isDamage;
            if (isDamage) {
                patch.Damage->Apply();
            } else {
                patch.Damage->Reset();
            }
            break;
        case 22:
            killself = !killself;
            if (killself) {
                patch.Killself->Apply();
            } else {
                patch.Killself->Reset();
            }
            suicideFall = !suicideFall;
            if (suicideFall) {
                patch.SuicideFall->Apply();
            } else {
                patch.SuicideFall->Reset();
            }
            imSuicide = !imSuicide;
            if (imSuicide) {
                patch.ImSuicide->Apply();
            } else {
                patch.ImSuicide->Reset();
            }
            suicideFallCoroutine = !suicideFallCoroutine;
            if (suicideFallCoroutine) {
                patch.SuicideFallCoroutine->Apply();
            } else {
                patch.SuicideFallCoroutine->Reset();
            }
            break;
        case 23:
            autoShootDelay = !autoShootDelay;
            if (autoShootDelay) {
                patch.AutoShootDelay->Apply();
            } else {
                patch.AutoShootDelay->Reset();
            }
            autoAimDistance = !autoAimDistance;
            if (autoAimDistance) {
                patch.AutoAimDistance->Apply();
            } else {
                patch.AutoAimDistance->Reset();
            }
            radiusAutoAim = !radiusAutoAim;
            if (radiusAutoAim) {
                patch.RadiusAutoAim->Apply();
            } else {
                patch.RadiusAutoAim->Reset();
            }
            effectiveDistance = !effectiveDistance;
            if (effectiveDistance) {
                patch.EffectiveDistance->Apply();
            } else {
                patch.EffectiveDistance->Reset();
            }
            minDistanceAutoAim = !minDistanceAutoAim;
            if (minDistanceAutoAim) {
                patch.MinDistanceAutoAim->Apply();
            } else {
                patch.MinDistanceAutoAim->Reset();
            }
            break;
        case 24:
            instantrespawn = !instantrespawn;
            if (instantrespawn) {
                patch.Instantrespawn->Apply();
            } else {
                patch.Instantrespawn->Reset();
            }
            break;
        case 25:
            ninjaJump = !ninjaJump;
            if (ninjaJump) {
                patch.NinjaJump->Apply();
            } else {
                patch.NinjaJump->Reset();
            }
            break;
        case 26:
            isBulletexplode = !isBulletexplode;
            break;
        case 27:
            isShotgun = !isShotgun;
            break;
        case 28:
            isReflectdamage = !isReflectdamage;
            isDamageReflectionValue = !isDamageReflectionValue;
            break;
        case 29:
            isRrailgun = !isRrailgun;
            break;
        case 30:
            isRocket = !isRocket;
            break;
        case 31:
            isAimbot = !isAimbot;
            break;
        case 32:
            isDash = !isDash;
            break;
        case 33:
            isMoney = !isMoney;
            break;
        case 34:
            isCharhed = !isCharhed;
            break;
        case 35:
            isJetPack = !isJetPack;
            break;
        case 36:
            isMushroom = !isMushroom;
            break;
        case 37:
            isChatSpamClan = !isChatSpamClan;
            break;
        case 38:
            isScaleUp = !isScaleUp;
            break;
        case 39:
            isScaleDown = !isScaleDown;
            break;
        case 40:
            isName = !isName;
            break;
        case 41:
            noclip = !noclip;
            break;
        case 42:
            isGadgetModificator = !isGadgetModificator;
            if (isGadgetModificator) {
                patch.GadgetModificator->Apply();
            } else {
                patch.GadgetModificator->Reset();
            }
            break;
        case 43:
            isRocketClip = !isRocketClip;
            if (isRocketClip) {
                patch.IsSkipCollision->Apply();
            } else {
                patch.IsSkipCollision->Reset();
            }
            break;
        case 44:
            isFly = !isFly;
            break;
        case 45:
            isReloadSpeed = !isReloadSpeed;
            if (isReloadSpeed) {
                patch.ReloadSpeed->Apply();
            } else {
                patch.ReloadSpeed->Reset();
            }
            break;
        default:
            break;
    }
    return;
  }

  void init(JNIEnv * env, jclass obj, jobject thiz){
    pthread_t ptid;
    pthread_create(&ptid, NULL, hack_thread, NULL);
    MakeToast(env, thiz, "ModMenu By Chewy - https://polarmods.com");
  }
  void changeSeekBar(JNIEnv *env, jclass clazz, jint i, jint seekbarValue) {
    int li = (int) i;
    switch (li) {
        case 46:
            levelModifier = seekbarValue;
            break;
        case 47:
            setkillsModifier = seekbarValue;
            break;
        default:
            break;
    }
    return;
  }

  void changeSpinner(JNIEnv *env, jclass clazz, jint i, jstring value) {
    int li = (int) i;
    switch (li) {
        case 0:
            break;
        default:
            break;
    }
  }

  void changeEditText(JNIEnv *env, jclass clazz, jint i, jstring value){
    int li = (int) i;
    switch (li){
        case 0:

        default:
            break;
    }
    return;
  }

  extern "C"
  JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* reserved) {
    JNIEnv* env;
    if (vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) != JNI_OK) {
        return JNI_ERR;
    }

    // Find your class. JNI_OnLoad is called from the correct class loader context for this to work.
    jclass c = env->FindClass("com/dark/force/NativeLibrary");
    if (c == nullptr) return JNI_ERR;

    // Register your class' native methods.
    static const JNINativeMethod methods[] = {
            {"changeEditText", "(ILjava/lang/String;)V", reinterpret_cast<void*>(changeEditText)},
            {"changeSeekBar", "(II)V", reinterpret_cast<void*>(changeSeekBar)},
            {"changeSpinner", "(ILjava/lang/String;)V", reinterpret_cast<void*>(changeSpinner)},
            {"changeToggle", "(I)V", reinterpret_cast<void*>(changeToggle)},
            {"getListFT", "()[Ljava/lang/String;", reinterpret_cast<void*>(getListFT)},
            {"init", "(Lcom/dark/force/MenuService;)V", reinterpret_cast<void*>(init)},
    };
    int rc = env->RegisterNatives(c, methods, sizeof(methods)/sizeof(JNINativeMethod));
    if (rc != JNI_OK) return rc;

    return JNI_VERSION_1_6;
  }
