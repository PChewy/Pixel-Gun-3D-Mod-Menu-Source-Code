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
#include "Includes/Chams.h"
#include "Includes/Color.h"
#include "Includes/Strings.h"


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
bool setkillsModifier = false;
bool isGod2 = false;
bool isSetBigHead = false;
bool isAddCoins = false;
bool isAddGems = false;
bool isAddCoupons = false;
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
bool isBulletexplode = false;
bool isShotgun = false;
bool isReflectdamage = false;
bool isRrailgun = false;
bool isRocket = false;
bool isDamageReflectionValue = false;
bool isFlamethrower = false;
bool isRange1 = false;
bool isRange2 = false;
bool isDash = false;
bool isMoney1 = false;
bool isMoney2 = false;
bool isCharhed = false;

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
}patch;

bool PlayerMoveCHookUpdateInitialized = false;
bool bankcontrollerhookinitialized = false;
bool weapsoundsHookInitialized = false;

void (*ActivateXRay)(void* _this, bool activate);
void (*SendChat)(void* _this, monoString* text, bool ClanLogo, monoString* iconName);
 /*
void (*set_curhealth)(void* _this, float amount);
void (*set_countkills)(void* _this, int amount);
void (*set_isImmortality)(void* _this, bool value);
void (*SetBigHeadEffect)(void* _this, bool value);
*/

void(*old_PlayerMoveCUpdate)(void *instance);
void PlayerMoveCUpdate(void *instance) {
    if(instance != NULL) {
        if(!PlayerMoveCHookUpdateInitialized){
            PlayerMoveCHookUpdateInitialized = true;
            LOGI("GameManager_LateUpdate hooked");
        }
        if (chams) {
            *(bool *) ((uint64_t) instance + 0x3A8) = true; //丑丛东世丅丑丛丘三
            ActivateXRay(instance, true);
        }
        if (isChatSpam) {
            SendChat(instance, CreateMonoString("Slice Cast Is My Dad!"), false, CreateMonoString("Slice Cast Is My Dad!"));
        }
        if (isInvisible) {
            *(bool *) ((uint64_t) instance + 0x2C6) = true; // 丛丞且丄丅丙业且丐
        }
        if (isVisible) {
            *(bool *) ((uint64_t) instance + 0x2C3) = false; // 业丁东丄丞与一丏上
             /*
        if (isGod) {
            set_curhealth(instance, 1.0f); //In-Game sets it at 2
        }
        if (setkillsModifier) {
            set_countkills(instance, (int) setkillsModifier);
        }
        if (isGod2) {
            set_isImmortality(instance, true);
        }
        if (isSetBigHead) {
            SetBigHeadEffect(instance, true);
        */
        }
    }
    old_PlayerMoveCUpdate(instance);
}

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
                (void (*)(void* _this, int, bool, int))getRealOffset(0x2B6B0B4); // BankController$$ 丒与丒业不丆丆丐业

                AddCoins(instance, 100, true, 1);
                isAddCoins = !AddCoins;
            }
        }
        if (isAddGems) {
            {
                void (*AddGems)(void* _this, int, bool, int) =
                (void (*)(void* _this, int, bool, int))getRealOffset(0x2B6B2F8); // BankController$$ 与丙万不丂丌丗业万

                AddGems(instance, 100, true, 1);
                isAddGems = !AddGems;
            }
        }
        if (isAddCoupons) {
            {
                void (*AddCoupons)(void* _this, int, bool, int) =
                (void (*)(void* _this, int, bool, int))getRealOffset(0x2B6B620); // BankController$$ 世七丁丛丙上且丂专

                AddCoupons(instance, 100, true, 1);
                isAddGems = !AddCoupons;
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
            *(bool *) ((uint64_t) instance + 0x105) = true; //bulletExplode
        }
        if (isShotgun) {
            *(bool *) ((uint64_t) instance + 0x106) = true; //isShotgun
        }
        if (isReflectdamage) {
            *(int *) ((uint64_t) instance + 0x229) = 50; //isDamageReflection
        }
        if (isRrailgun) {
            *(bool *) ((uint64_t) instance + 0x110) = true; //railgun
        }
        if (isRocket) {
            *(bool *) ((uint64_t) instance + 0xA4) = true; //bazooka
        }
        if (isDamageReflectionValue) {
            *(float *) ((uint64_t) instance + 0x22C) = 200.0; //damageReflectionValue
        }
        if (isFlamethrower) {
            *(bool *) ((uint64_t) instance + 0x260) = true; //flamethrower
        }
        if (isRange1) {
            *(float *) ((uint64_t) instance + 0x3C8) = 12313312; //range
        }
        if (isRange2) {
            *(float *) ((uint64_t) instance + 0xE8) = 14121242; //radiusRoundMelee
        }
        if (isDash) {
            *(bool *) ((uint64_t) instance + 0x1CC) = true; //isDash
        }
        if (isMoney1) {
            *(float *) ((uint64_t) instance + 0x248) = 3123112; //coinDropChance
        }
        if (isMoney2) {
            *(bool *) ((uint64_t) instance + 0x244) = true; //isCoinDrop
        }
        if (isCharhed) {
            *(float *) ((uint64_t) instance + 0x134) = 0; //chargeTime
        }
    }
    old_WeapSoundsUpdate(instance);
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
    ActivateXRay = (void (*)(void*, bool))getRealOffset(0x1E3F720); //且丂丙丌上丕丛丞丙
    SendChat = (void (*)(void*, monoString*, bool, monoString*))getRealOffset(0x1E2047C); //SendChat:Player_move_c
    /*
    set_countkills = (void (*)(void*, int))getRealOffset(0xFFFFFFF);
    set_curhealth = (void (*)(void*, float))getRealOffset(0xFFFFFF);
    set_isImmortality = (void (*)(void*, bool))getRealOffset(0xFFFFFF);
    SetBigHeadEffect = (void (*)(void*, bool))getRealOffset(0xFFFFFF);
    */
    octo_hook((void*)getRealOffset(0x1E4CE74), (void*)PlayerMoveCUpdate, (void**)&old_PlayerMoveCUpdate);
    octo_hook((void*)getRealOffset(0x2B663EC), (void*)BankControllerUpdate, (void**)&old_BankControllerUpdate);
    octo_hook((void*)getRealOffset(0x229CD44), (void*)WeapSoundsUpdate, (void**)&old_WeapSoundsUpdate);

    patch.Recoil = Patch::Setup((void*)getAbsoluteAddress(0x318D604), (char*)"\x01\x00\xa0\xe3\x1e\xff\x2f\xe1", 8); //丅三丑一丂丈丕丁丅
    patch.ScatterReduction = Patch::Setup((void*)getAbsoluteAddress(0x318CE14), (char*)"\x01\x00\xa0\xe3\x1e\xff\x2f\xe1", 8); //丂丛丐专丛下丐丏丅
    patch.CriticalShot = Patch::Setup((void*)getAbsoluteAddress(0x318D064), (char*)"\x01\x00\xa0\xe3\x1e\xff\x2f\xe1", 8); //丕下业与丁丐丛下东
    patch.RegenHealth = Patch::Setup((void*)getAbsoluteAddress(0x3186F94), (char*)"\x01\x00\xa0\xe3\x1e\xff\x2f\xe1", 8); //专丅万专不丙丒丟与
    patch.RegenArmor = Patch::Setup((void*)getAbsoluteAddress(0x3186F08), (char*)"\x01\x00\xa0\xe3\x1e\xff\x2f\xe1", 8); //丞丄丂上三丁不丗丏
    patch.Ammo1 = Patch::Setup((void*)getAbsoluteAddress(0x3186F94), (char*)"\x64\x00\xa0\xe3\x1e\xff\x2f\xe1", 8); //get_currentAmmoInBackpack | 丆丞东丂上与丏不丟
    patch.Ammo2 = Patch::Setup((void*)getAbsoluteAddress(0x3186F94), (char*)"\x64\x00\xa0\xe3\x1e\xff\x2f\xe1", 8); //get_currentAmmoInClip | 不业且丘三丗丆丛丗
    patch.Level = Patch::Setup((void*)getAbsoluteAddress(0x2CB39D8), (char*)"\x00\x00\xA0\xE3\x1E\xFF\x2F\xE1", 8); // 三丛且七丕上丂丁丅
    patch.AutoShoot = Patch::Setup((void*)getAbsoluteAddress(0x1E88488), (char*)"\x01\x00\xa0\xe3\x1e\xff\x2f\xe1", 8); // 丄世丞丗万丒丕丑专
    patch.Vip = Patch::Setup((void*)getAbsoluteAddress(0x34F0408), (char*)"\x01\x00\xa0\xe3\x1e\xff\x2f\xe1", 8); // 丙世丘丙丕丒上世上
    patch.Rapidfire = Patch::Setup((void*)getAbsoluteAddress(0x20C8874), (char*)"\xFA\x04\x44\xE3\x1E\xFF\x2F\xE1", 8); // 上丄丈东丁与七丅专
    patch.Damage = Patch::Setup((void*)getAbsoluteAddress(0x1E68DF0), (char*)"\xFA\x04\x44\xE3\x1E\xFF\x2F\xE1", 8); // 不丟不丂东下万丙不
    return NULL;
}

jobjectArray getListFT(JNIEnv *env, jclass jobj){
   jobjectArray ret;
   int i;
   const char *features[]= {
           "Chams",
           "Chat Spam",
           "Invisible",
           "GodMode",
           "Set Kills",
           "GodMode V2",
           "Big Head",
           "Add Coins",
           "Add Gems",
           "Add Coupons",
           "No Recoil",
           "No Spread",
           "Critical Hit",
           "Inf Ammo",
           "Health Regeneration",
           "Armor Regeneration",
           "Don't Level Up",
           "Auto Shoot",
           "Vip",
           "Firerate",
           "One Hit Kill",
           "Shoot Shotgun Bullets",
           "Thorns",
           "Shoot Laser Bullet",
           "Shoot Rockets",
           "Silent AimBot",
           "Shoot Dash",
           "Enemies Drop Coins When You Kill Them",
           "No Charge Time",
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
           setkillsModifier = !setkillsModifier;
           break;
       case 5:
           isGod2 = !isGod2;
           break;
       case 6:
           isSetBigHead = !isSetBigHead;
           break;
       case 7:
           isAddCoins = !isAddCoins;
           break;
       case 8:
           isAddGems = !isAddGems;
           break;
       case 9:
           isAddCoupons = !isAddCoupons;
           break;
       case 10:
           isRecoil = !isRecoil;
           if (isRecoil) {
               patch.Recoil->Apply();
           } else {
               patch.Recoil->Reset();
           }
           break;
       case 11:
           isScatterReduction = !isScatterReduction;
           if (isScatterReduction) {
               patch.ScatterReduction->Apply();
           } else {
               patch.ScatterReduction->Reset();
           }
       case 12:
           isCriticalHit = !isCriticalHit;
           if (isCriticalHit) {
               patch.CriticalShot->Apply();
           } else {
               patch.CriticalShot->Reset();
           }
           break;
       case 13:
           isRegenHealth = !isRegenHealth;
           if (isRegenHealth) {
               patch.RegenHealth->Apply();
           } else {
               patch.RegenHealth->Reset();
           }
           break;
       case 14:
           isRegenArmor = !isRegenArmor;
           if (isRegenArmor) {
               patch.RegenArmor->Apply();
           } else {
               patch.RegenArmor->Reset();
           }
           break;
       case 15:
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
       case 16:
           isLevel = !isLevel;
           if (isLevel) {
               patch.Level->Apply();
           } else {
               patch.Level->Reset();
           }
           break;
       case 17:
           isAutoShoot = !isAutoShoot;
           if (isAutoShoot) {
               patch.AutoShoot->Apply();
           } else {
               patch.AutoShoot->Reset();
           }
           break;
       case 18:
           isVip = !isVip;
           if (isVip) {
               patch.Vip->Apply();
           } else {
               patch.Vip->Reset();
           }
           break;
       case 19:
           isRapidfire = !isRapidfire;
           if (isRapidfire) {
               patch.Rapidfire->Apply();
           } else {
               patch.Rapidfire->Reset();
           }
           break;
       case 20:
           isDamage = !isDamage;
           if (isDamage) {
               patch.Damage->Apply();
           } else {
               patch.Damage->Reset();
           }
           break;
       case 21:
           isBulletexplode = !isBulletexplode;
           break;
       case 22:
           isShotgun = !isShotgun;
           break;
       case 23:
           isReflectdamage = !isReflectdamage;
           isDamageReflectionValue = !isDamageReflectionValue;
           break;
       case 24:
           isRrailgun = !isRrailgun;
           break;
       case 25:
           isRocket = !isRocket;
           break;
       case 26:
           isFlamethrower = !isFlamethrower;
           isRange1 = !isRange1;
           isRange2 = !isRange2;
           break;
       case 27:
           isDash = !isDash;
           break;
       case 28:
           isMoney1 = !isMoney1;
           isMoney2 = !isMoney2;
           break;
       case 29:
           isCharhed = !isCharhed;
           break;
       default:
           break;
   }
   return;
}

void init(JNIEnv * env, jclass obj, jobject thiz){
   pthread_t ptid;
   pthread_create(&ptid, NULL, hack_thread, NULL);
   MakeToast(env, thiz, "ModMenu By Chewy & SliceCast - https://polarmods.com");
}

void changeSeekBar(JNIEnv *env, jclass clazz, jint i, jint seekbarValue) {
   int li = (int) i;
   switch (li) {
           break;
       default:
           break;
   }
   return;
}

void changeSpinner(JNIEnv *env, jclass clazz, jint i, jstring value) {
   int li = (int) i;
   switch (li) {
       default:
           break;
   }
}

void changeEditText(JNIEnv *env, jclass clazz, jint i, jstring value){
   int li = (int) i;
   switch (li){
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
