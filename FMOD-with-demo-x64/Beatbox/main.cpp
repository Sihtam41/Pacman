#include <iostream>
#include <thread>
#include <fmod.h>

using namespace std;

int main()
{
    FMOD_SYSTEM *system;
    FMOD_SOUND *soundKick, *soundSnare, *soundHihat;
    FMOD_RESULT result;

    result = FMOD_System_Create(&system);
    FMOD_System_Init(system, 8, FMOD_INIT_NORMAL, nullptr);

    // Grosse caisse
    result = FMOD_System_CreateSound(system, "./data/kick.wav", FMOD_SOFTWARE, nullptr, &soundKick);
    if (result!=FMOD_OK)
    {
        cout<<"Cannot create sound. Error"<<result<<endl;
        return -1;
    }

    // Caisse claire
    result = FMOD_System_CreateSound(system, "./data/snare.wav", FMOD_SOFTWARE, nullptr, &soundSnare);
    if (result!=FMOD_OK)
    {
        cout<<"Cannot create sound. Error"<<result<<endl;
        return -1;
    }

    // Charle
    result = FMOD_System_CreateSound(system, "./data/hihat.wav", FMOD_SOFTWARE, nullptr, &soundHihat);
    if (result!=FMOD_OK)
    {
        cout<<"Cannot create sound. Error"<<result<<endl;
        return -1;
    }

    // Tempo (nombre de noires par minute)
    int tempoBPM = 120;

    // Durée d'une noire en millisecondes
    int msQuarterNote = 60000/tempoBPM;

    // 2 mesures de disco...
    for (int halfBar=0; halfBar<4; halfBar++)
    {
        // Grosse caisse, croche
        FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, soundKick, 0, NULL);
        this_thread::sleep_for(chrono::milliseconds(msQuarterNote/2));

        // Charle, 2 double-croches
        FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, soundHihat, 0, NULL);
        this_thread::sleep_for(chrono::milliseconds(msQuarterNote/4));
        FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, soundHihat, 0, NULL);
        this_thread::sleep_for(chrono::milliseconds(msQuarterNote/4));

        // Caisse claire, croche
        FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, soundSnare, 0, NULL);
        this_thread::sleep_for(chrono::milliseconds(msQuarterNote/2));

        // Charle, croche
        FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, soundHihat, 0, NULL);
        this_thread::sleep_for(chrono::milliseconds(msQuarterNote/2));
    }

    FMOD_Sound_Release(soundKick);
    FMOD_Sound_Release(soundSnare);
    FMOD_Sound_Release(soundHihat);
    FMOD_System_Close(system);

    return 0;
}
