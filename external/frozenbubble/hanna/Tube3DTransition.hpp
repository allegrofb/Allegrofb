#ifndef TUBE3DTRANSITION_H
#define TUBE3DTRANSITION_H

#include "Alo.hpp"
#include "Drawable.hpp"

class Tube3DTransition
{
public:
	Tube3DTransition();
	~Tube3DTransition();

public:
	enum MODE
	{
		TO_RIGHT,			//front->left
		TO_LEFT,			//front->right
		TO_LEFT_BACK,		//front->left->back
		TO_RIGHT_BACK,		//front->right->back
		TO_LEFT_FRONT,		//front->left->front
		TO_RIGHT_FRONT,		//front->right->front
	};

	enum CUBESIDE
	{
		TOP,
		BOTTOM,
		FRONT,
		RIGHT,
		BACK,
		LEFT
	};

public:
	void Init(int w, int h, MODE mode, double frameRate, ALLEGRO_BITMAP* others);
	void UpdateFirst(Drawable* p,int offsetx,int offsety, double zoom);
	void UpdateSecond(Drawable* p,int offsetx,int offsety, double zoom);
	void Prepare();
	void Render(int x, int y, double zoom);
	bool IsFinished();
	void Finish();
	void CalculateCamera(MODE mode);
private:
	void MovingCamera();
	ALLEGRO_BITMAP* GetFirst(int w, int h);
	ALLEGRO_BITMAP* GetSecond(int w, int h);
private:
	int width;
	int height;
private:
	struct camera
	{
		double xangle, yangle, zangle;
		double dist;
		double factor;
	};
    struct tex_coordinate
    {
        tex_coordinate():x0(0),x1(1),y0(0),y1(1){}
        GLfloat x0,x1,y0,y1;                    
    };

	int current_frame;
	int frame_num;
	int intermediate_num;
	camera *data_p;
	GLuint tex_array[6];	
    tex_coordinate tex_coords[6];
	ALLEGRO_BITMAP* bitmap_first;
	ALLEGRO_BITMAP* bitmap_second;
    tex_coordinate tex_coords_1st;
    tex_coordinate tex_coords_2nd;
	GLuint tex_first;
	GLuint tex_second;

	//setup parameter
	ALLEGRO_BITMAP* bitmap_other;
	GLuint tex_other;
	MODE transition_mode;

	double scale_orignal ;
	double scale_dithering_value ;

	double angley_start ;
	double angley_end ;

	double anglex_orignal ;
	double anglex_dithering_value ;

	double transition_time ;
	double frame_rate ;

	double intermediate_delay;
};




#endif	//TUBE3DTRANSITION_H

