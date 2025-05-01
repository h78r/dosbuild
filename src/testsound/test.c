#include <stdio.h>
#include "mikmod.h"
#include "xm_data.h"  // <- your embedded module

int main(void) {
    MODULE *module;

    MikMod_RegisterAllDrivers();
    MikMod_RegisterAllLoaders();

    if (MikMod_Init("")) {
        printf("MikMod init failed: %s\n", MikMod_strerror(MikMod_errno));
        return 1;
    }

    // Use Player_LoadMem instead of Player_Load
    module = Player_LoadMem(xm_data, xm_data_len, 64, 0);
    if (!module) {
        printf("Error loading embedded XM: %s\n", MikMod_strerror(MikMod_errno));
        MikMod_Exit();
        return 1;
    }

    Player_Start(module);
    printf("Playing embedded XM...\n");

    while (Player_Active()) {
        MikMod_Update();
    }

    Player_Stop();
    Player_Free(module);
    MikMod_Exit();

    return 0;
}
