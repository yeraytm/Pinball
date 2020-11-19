#pragma once
#ifndef __MODULE_AUDIO_H__
#define __MODULE_AUDIO_H__

#include "Module.h"
#include "SDL_mixer\include\SDL_mixer.h"

#define DEFAULT_MUSIC_FADE_TIME 2.0f

class ModuleAudio : public Module
{
public:
	ModuleAudio(Application* app, bool start_enabled = true);
	~ModuleAudio();

	bool Start();
	bool CleanUp();

	// Play a music file
	bool PlayMusic(const char* path, float fade_time = DEFAULT_MUSIC_FADE_TIME);

	// Load a WAV in memory
	unsigned int LoadFx(const char* path);

	// Play a previously loaded WAV
	bool PlayFx(unsigned int fx, int repeat = 0);
	bool StopMusic();
	bool MuteMusic();
	bool MusicIsPaused() {
		return Mix_PausedMusic();
	}

	// Methods to control the volume
	bool VolumeChange(int volume);
	void VolumeLimiter();

private:
	Mix_Music*			music;
	p2List<Mix_Chunk*>	fx;

public:
	int volume;
};

#endif // __MODULE_AUDIO_H__