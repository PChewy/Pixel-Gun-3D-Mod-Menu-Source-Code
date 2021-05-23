# Pixel-Gun-3D-Mod-Menu-Source-Code

Source Code for PG3D 21.3.0.

Made by Chewy

Use this mod-menu template (only supported) https://github.com/Octowolve/Substrate-Template-With-Mod-Menu

Credits: SliceCast / Chewy / SliceCast / Jbro129 <3


# WARNING

1. Hooking doesn't work on this game (idk why)

2. for speed speed modifier and jump modifier you have to patch/KittyMemory it as a slider, Names for speed and jump: SpeedModifier$$上丈世丐不丁且丅丕 - 七丝丈上丂丙丘丅上$$JumpModifier

Use https://github.com/LGLTeam/Android-Mod-Menu if you want to do this

```cpp
            if (sliderValue == 0) {
                my_cool_Patches.Level.Restore();
            } else if (sliderValue == 1) {
                my_cool_Patches.Level = MemoryPatch(libName, 0x164293C,
                                                    "\x01\x00\xA0\xE3\x1E\xFF\x2F\xE1", 8);
                my_cool_Patches.Level.Modify();
            } else if (sliderValue == 2) {
                my_cool_Patches.Level = MemoryPatch(libName, 0x164293C,
                                                    "\x0F\x00\xA0\xE3\x1E\xFF\x2F\xE1", 8);
                my_cool_Patches.Level.Modify();
            } else if (sliderValue == 3) {
                my_cool_Patches.Level = MemoryPatch(libName, 0x164293C,
                                                    "\x23\x00\xA0\xE3\x1E\xFF\x2F\xE1", 8);
                my_cool_Patches.Level.Modify();
            } else if (sliderValue == 4) {
                my_cool_Patches.Level = MemoryPatch(libName, 0x164293C,
                                                    "\x37\x00\xA0\xE3\x1E\xFF\x2F\xE1", 8);
                my_cool_Patches.Level.Modify();
            } else if (sliderValue == 5) {
                my_cool_Patches.Level = MemoryPatch(libName, 0x164293C,
                                                    "\x41\x00\xA0\xE3\x1E\xFF\x2F\xE1", 8);
                my_cool_Patches.Level.Modify();
            }
```

6. Chat spam is using "Start" because there isn't any update function in that class, this means that you may have to reopen the game or clans for it to work because it only gets called ones
