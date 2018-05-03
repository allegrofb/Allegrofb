#ifndef SAMPLE_H
#define SAMPLE_H

#include "Alo.hpp"
#include "Singleton.hpp"

#define RESERVED_SAMPLES   16
#define MAX_SAMPLE_DATA    16

class Sample_
{
public:
	bool Play(int i, int mode=ALLEGRO_PLAYMODE_ONCE);
	void Stop(int i);
	void Stop();
	bool LoadFrom(const char* filenames[]);
private:
	void Init();
	ALLEGRO_SAMPLE *sample_data[MAX_SAMPLE_DATA];
	ALLEGRO_SAMPLE_ID sample_data_id[MAX_SAMPLE_DATA];
	bool sample_data_id_valid[MAX_SAMPLE_DATA];
	bool myValid;
};

typedef singleton_default<Sample_> Sample;




#define MAX_STREAM_DATA    16

class Stream_
{
public:
	bool Play(int i, ALLEGRO_PLAYMODE mode=ALLEGRO_PLAYMODE_LOOP);
	void Stop(int i);
	void Stop();
	bool LoadFrom(const char* filenames[]);
	void Destroy();
private:
	void Init();
	ALLEGRO_AUDIO_STREAM *stream_data[MAX_STREAM_DATA];
	bool stream_data_id_valid[MAX_STREAM_DATA];
	bool myValid;
	ALLEGRO_MIXER*  mixer;
};

typedef singleton_default<Stream_> Stream;

#endif	//SAMPLE_H

