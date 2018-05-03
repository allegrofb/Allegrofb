#include "Stream.hpp"

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
	
//	if(stream_data_id_valid[i])
//		return true;

    if(!stream_data_id_valid[i])
    {
        al_set_audio_stream_playmode(stream_data[i], mode);
        al_attach_audio_stream_to_mixer(stream_data[i], mixer);
        stream_data_id_valid[i] = true;
	}
    else
    {
        al_set_audio_stream_playing(stream_data[i],true);
    }
    
	return true;
}

void Stream_::Stop(int i)
{
	if(i >= MAX_STREAM_DATA || i < 0 || !myValid)
		return;
    
//	if(stream_data_id_valid[i])
	{
        al_set_audio_stream_playing(stream_data[i],false);
//        al_drain_audio_stream(stream_data[i]);
//		al_detach_audio_stream(stream_data[i]);
//		stream_data_id_valid[i] = false;
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

void Stream_::Release()
{
	for(int i = 0;i < MAX_STREAM_DATA;i++)
	{
		if(stream_data[i])
			al_destroy_audio_stream(stream_data[i]);
	}
}
