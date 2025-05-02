#include <stdio.h>      // Provides standard input/output functions (e.g., printf)
#include "mikmod.h"     // MikMod library header — provides music module playback API
#include "track1_xm_data.h"   // Contains xm_data1[] array (raw music module bytes) and xm_data1_len (its length)
#include "track2_xm_data.h"   // Contains xm_data1[] array (raw music module bytes) and xm_data1_len (its length)
#include "track3_xm_data.h"   // Contains xm_data1[] array (raw music module bytes) and xm_data1_len (its length)
#include <dos.h>        // Provides DOS-specific functions (not directly used here, included for legacy compatibility)

// Define a structure to store both a pointer to music data and its length
// MikMod requires both to load a module from memory correctly
typedef struct {
    const void *data;   // Pointer to the music module data in memory (e.g. .XM or .MOD format)
    long length;        // Length of the module data in bytes
} ModuleData;

void clearSCR(){
    int i; 
    for (i=0; i < 100; i++){
        printf("\n");
    }
}

int main(void) {

    MODULE *module;  // Pointer to a MikMod MODULE structure — stores internal state for the currently loaded song

    // Array of modules to be played in sequence
    // Each item contains a memory pointer to the embedded music file and its size
    ModuleData tracks[] = {
        { track1_xm_data, track1_xm_data_len },  // First track: embedded .XM module
        { track2_xm_data, track2_xm_data_len },  // Second track: embedded .XM module
        { track3_xm_data, track3_xm_data_len },  // Third track: embedded .XM module


        // You can add more tracks here if needed
    };

    // Calculate the number of tracks in the array using the size of the array and the size of one element
    int numTracks = sizeof(tracks) / sizeof(ModuleData);
    int trackIndex = 0;  // Index of the current track being played

    // Register all available audio output drivers (e.g., Sound Blaster, GUS, etc.)
    // Required before calling MikMod_Init
    MikMod_RegisterAllDrivers();

    // Register all module file format loaders (XM, MOD, S3M, IT, etc.)
    // This ensures MikMod can recognize and decode various tracker formats
    MikMod_RegisterAllLoaders();

    // Initialize the MikMod audio system with default parameters (empty string = default device)
    if (MikMod_Init("")) {
        // If initialization fails, display an error and exit
        printf("Error starting H78 Player for DOS: %s\n", MikMod_strerror(MikMod_errno));
        return 1;
    }

    //clearSCR(); //clear the screen
    // Inform the user that playback has started
    printf("Release Player Lib - H78 2005.\n");

    // Main loop: play all tracks in order, then repeat forever
    while (1) {
        // Load the current track from memory into a new MikMod module structure
        // Player_LoadMem parses the data and allocates memory for internal use
        module = Player_LoadMem(tracks[trackIndex].data, tracks[trackIndex].length, 64, 0);
        if (!module) {
            // If loading fails, print the error and clean up
            printf("Error loading tracker data: %s\n", MikMod_strerror(MikMod_errno));
            MikMod_Exit();  // Shut down audio system
            return 1;
        }

        // Disable module wrapping — prevents song from jumping back to the start when it ends
        module->wrap = 0;

        // Disable module looping — we want to play each song once, then move to the next
        module->loop = 0;

        // Start playback of the loaded module
        Player_Start(module);

        // Playback loop: while the song is active (not finished), keep calling MikMod_Update
        // MikMod_Update must be called frequently to generate audio and handle playback logic
        while (Player_Active()) {
            MikMod_Update();  // Feeds data to the sound card and advances song position
        }

        // When playback finishes, stop the module
        Player_Stop();

        // Free all memory associated with the module (internal song structure and audio buffers)
        Player_Free(module);

        // Advance to the next track in the array
        // If we're at the last track, wrap back to the first one (infinite playlist loop)
        trackIndex = (trackIndex + 1) % numTracks;
    }

    // Unreachable in this version — program runs forever
    MikMod_Exit();  // Clean up MikMod resources (never reached)
    return 0;       // Return success (also never reached)
}
