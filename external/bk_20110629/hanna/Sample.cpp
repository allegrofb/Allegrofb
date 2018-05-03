#include "Sample.hpp"

void Sample_::Init()
{
	myValid = false;
	memset(sample_data, 0, sizeof(sample_data));
	memset(sample_data_id, 0, sizeof(sample_data_id));

	if (!al_reserve_samples(RESERVED_SAMPLES)) {
		debug_message("Could not set up voice and mixer.\n");
		return;
	}

	myValid = true;
}

bool Sample_::LoadFrom(const char* filenames[])
{
	bool r = true;

	Init();

	if(!myValid)
		return false;

	for(int i = 0;filenames[i];i++)
	{
		sample_data_id_valid[i] = false;
		sample_data[i] = al_load_sample(util_get_path(filenames[i]));
		if (!sample_data[i]) 
		{
			r = false;
			debug_message( "Could not load sample from '%s'!\n", filenames[i]);
			continue;
		}
	}

	return r;
}

bool Sample_::Play(int i, int mode)
{
	if(i >= MAX_SAMPLE_DATA || i < 0 || !myValid)
		return false;

	if (!sample_data[i])
		return false;

	if (!al_play_sample(sample_data[i], 1.0, 0.5, 1.0, (ALLEGRO_PLAYMODE)mode, &sample_data_id[i])) {
		sample_data_id_valid[i] = false;
		debug_message("al_play_sample_data failed, perhaps too many sounds\n");
		return false;
	}
	sample_data_id_valid[i] = true;
	return true;
}

void Sample_::Stop(int i)
{
	if(i >= MAX_SAMPLE_DATA || i < 0 || !myValid)
		return;
	if(sample_data_id_valid[i])
	{
		al_stop_sample(&sample_data_id[i]);
	}
}

void Sample_::Stop()
{
	if(!myValid)
		return;

	al_stop_samples();
}




void Stream_::Init()
{
	myValid = false;
	memset(stream_data, 0, sizeof(stream_data));

	mixer = al_get_default_mixer();
	if (mixer == NULL) {
		debug_message("Could not get default mixer.\n");
		return;
	}

	myValid = true;
}

bool Stream_::LoadFrom(const char* filenames[])
{
	bool r = true;

	Init();

	if(!myValid)
		return false;

	for(int i = 0;filenames[i];i++)
	{
		stream_data_id_valid[i] = false;
		stream_data[i] = al_load_audio_stream(util_get_path(filenames[i]),4,2048);
		if (!stream_data[i]) 
		{
			r = false;
			debug_message( "Could not load audio stream from '%s'!\n", filenames[i]);
			continue;
		}
	}

	return r;
}

bool Stream_::Play(int i, ALLEGRO_PLAYMODE mode)
{
	if(i >= MAX_STREAM_DATA || i < 0 || !myValid)
		return false;

	if (!stream_data[i])
		return false;
	
	if(stream_data_id_valid[i])
		return true;

	al_set_audio_stream_playmode(stream_data[i], mode);
	al_attach_audio_stream_to_mixer(stream_data[i], mixer);
	stream_data_id_valid[i] = true;
	
	return true;
}

void Stream_::Stop(int i)
{
	if(i >= MAX_STREAM_DATA || i < 0 || !myValid)
		return;
	if(stream_data_id_valid[i])
	{
		al_detach_audio_stream(stream_data[i]);
		stream_data_id_valid[i] = false;
	}
}

void Stream_::Stop()
{
	if(!myValid)
		return;

	for(int i = 0;i < MAX_STREAM_DATA;i++)
	{
		Stop(i);
	}
}

void Stream_::Destroy()
{
	for(int i = 0;i < MAX_STREAM_DATA;i++)
	{
		if(stream_data[i])
			al_destroy_audio_stream(stream_data[i]);
	}
}


