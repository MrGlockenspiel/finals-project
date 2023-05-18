#include <SDL2/SDL.h>
#include <ncurses.h>

#include <rogue.h>

SDL_AudioSpec want, have;
SDL_AudioDeviceID dev;
SDL_AudioSpec music_spec;
Uint8 *music_data;
Uint32 music_length;

int init_audio() {
    if (SDL_Init(SDL_INIT_AUDIO) != 0) {
        write_log(SDL_GetError());
        return 1;
    }

    SDL_memset(&want, 0, sizeof(want));
    want.freq = 16000;
    want.format = AUDIO_S16SYS;
    want.channels = 2;
    want.samples = 1024;
    want.callback = NULL;
    want.userdata = NULL;

    dev = SDL_OpenAudioDevice(NULL, 0, &want, &have, 0);
    if (dev == 0) {
        write_log(SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_PauseAudioDevice(dev, 0);
    return 0;
}

void play_sound_effect(const char* filepath) {
    char err[] = "Audio: Error loading wav: ";
    SDL_AudioSpec spec;
    Uint32 length;
    Uint8 *data = malloc(sizeof(Uint8));

    spec.freq = 16000;
    
    if (SDL_LoadWAV(filepath, &spec, &data, &length) == NULL) {
        write_log(strcat(err, filepath));
        return;
    }

    SDL_QueueAudio(dev, data, length);
    SDL_FreeWAV(data);

}

void play_string(char* string) {
    for (int i = 0; i < strlen(string); i++) {
        play_char(string[i]);
    }
}

void play_char(char ch) {
    char path[64];
    char err[] = "Audio: Error loading wav: ";
    SDL_AudioSpec spec;
    Uint32 length;
    Uint8 *data = malloc(sizeof(Uint8));

    spec.freq = 16000;

    if (ch < 'A' || ch > 'Z') return;

    sprintf(path, "sound/npc/%c.wav", ch);

    if (SDL_LoadWAV(path, &spec, &data, &length) == NULL) {
        
        write_log(strcat(err, path));
        return;
    }

    SDL_QueueAudio(dev, data, length);
    SDL_FreeWAV(data);

    while (SDL_GetQueuedAudioSize(dev) > 0) {
        SDL_Delay(10);
    }
}


int load_music(const char* filepath) {
    
    if (SDL_LoadWAV(filepath, &music_spec, &music_data, &music_length) == NULL) {
        write_log(SDL_GetError());
        return 1;
    }

    //music_spec.callback = NULL;
    //music_spec.userdata = NULL;

    return 0;
}

int play_music() {
    SDL_AudioDeviceID music_dev = SDL_OpenAudioDevice(NULL, 0, &music_spec, &have, 0);
    if (music_dev == 0) {
        write_log(SDL_GetError());
        return 1;
    }

    SDL_QueueAudio(music_dev, music_data, music_length);
    SDL_PauseAudioDevice(music_dev, 0);

    return 0;
}

