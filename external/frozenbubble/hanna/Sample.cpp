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

void Sample_::Release()
{
    for(int i = 0;i < MAX_SAMPLE_DATA;i++)
    {
        if(sample_data[i])
            al_destroy_sample(sample_data[i]);
    }
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

