#ifndef STREAM_H
#define STREAM_H

#include "Alo.hpp"
#include "Singleton.hpp"

#define MAX_STREAM_DATA    16

class Stream_
{
public:
	bool Play(int i, ALLEGRO_PLAYMODE mode=ALLEGRO_PLAYMODE_LOOP);
	void Stop(int i);
	void Stop();
	bool LoadFrom(const char* filenames[]);
	void Release();
private:
	void Init();
	ALLEGRO_AUDIO_STREAM *stream_data[MAX_STREAM_DATA];
	bool stream_data_id_valid[MAX_STREAM_DATA];
	bool myValid;
	ALLEGRO_MIXER*  mixer;
};

typedef singleton_default<Stream_> Stream;

#endif	//STREAM_H

