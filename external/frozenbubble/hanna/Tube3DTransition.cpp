#include "Tube3DTransition.hpp"
#include <math.h>


Tube3DTransition::Tube3DTransition()
	: data_p(NULL)
{
	scale_orignal = 1;
//	scale_dithering_value = 0.85;
	scale_dithering_value = 0.85;

	angley_start = 0;
	angley_end = -90.0;		//-90, to left, 90, to right

	anglex_orignal = 0;
//	anglex_dithering_value = 6.0;
	anglex_dithering_value = 7.0;

	intermediate_delay = 0.35;
	transition_time = 0.8;
	frame_rate = 30.0;

	bitmap_first	= NULL;
	bitmap_second	= NULL;
	bitmap_other	= NULL;
	tex_first		= 0;
	tex_second		= 0;
	tex_other		= 0;

	for(int i = 0;i<6;i++)
		tex_array[i] = 0;

	transition_mode = TO_RIGHT;
}

Tube3DTransition::~Tube3DTransition()
{
	if(data_p)
		delete[] data_p;
//
//	GLuint tmp[] = {tex_other,tex_first,tex_second};
//
//	glDeleteTextures(3,tmp);
//
//	if(bitmap_other)
//		al_destroy_bitmap(bitmap_other);
	if(bitmap_first)
		al_destroy_bitmap(bitmap_first);
	if(bitmap_second)
		al_destroy_bitmap(bitmap_second);
}

void Tube3DTransition::SetTransitionTime(double time)
{
    this->transition_time = time;
}


void Tube3DTransition::Init(int w, int h, MODE mode, double frameRate, ALLEGRO_BITMAP* others)
{
	this->transition_mode = mode;
	this->frame_rate = frameRate;
	width = w;
	height = h;

	CalculateCamera(mode);

	if(others)
	{
//		if(bitmap_other)
//			al_destroy_bitmap(bitmap_other);

		bitmap_other = others;

//		if(tex_other)
//		{
//			GLuint tmp[] = {tex_other};
//			glDeleteTextures(1,tmp);
//		}

		tex_other = al_get_opengl_texture(others);
        int other_w = w, other_h = h;
        int t_w = 0,t_h = 0;
        
        other_h = al_get_bitmap_height(this->bitmap_other);
        other_w = al_get_bitmap_width(this->bitmap_other);        
        al_get_opengl_texture_size(this->bitmap_other, &t_w, &t_h);
        
        if(t_w && t_h)
        {
            tex_coords_other.x1 = (GLfloat)other_w/(GLfloat)t_w;
            tex_coords_other.y1 = (GLfloat)other_h/(GLfloat)t_h;
        }                
	}
}

void Tube3DTransition::CalculateCamera(MODE mode)
{
	#define PI 3.14159265
	int i = 0;
	int num = 0;
	double y_end = angley_end;
	double y_start = angley_start;

	this->intermediate_num = 0;
	this->transition_mode  = mode;

	num = frame_rate*transition_time;

	if(mode == TO_RIGHT || mode == TO_LEFT)
	{
		frame_num = num;
	}
	else
	{
		frame_num = 2*num;
	}

	if(mode == TO_RIGHT || mode == TO_RIGHT_FRONT)
		y_end = -angley_end;

	if(data_p)
		delete[] data_p;

	data_p  = new camera[frame_num];

	if(mode == TO_RIGHT || mode == TO_LEFT || mode == TO_LEFT_FRONT || mode == TO_RIGHT_FRONT)
	{
		//now, only transition one side, to do
		for(i = 0;i<num;i++)
		{
			data_p[i].xangle = anglex_orignal + 
				cos(PI*(float)(i+1)/(float)num - PI/2)*(anglex_dithering_value-anglex_orignal);  
			data_p[i].yangle = (sin(PI*(float)(i+1)/(float)num - PI/2)+1)/2*(y_end-y_start);
			data_p[i].zangle = 0;
			data_p[i].dist = -3;
			data_p[i].factor = scale_orignal + 
				cos((float)(i+1)/(float)num*PI - PI/2)*(scale_dithering_value-scale_orignal);
		}
	}

	if(mode == TO_LEFT_FRONT || mode == TO_RIGHT_FRONT)
	{
		this->intermediate_num = intermediate_delay*frame_rate;
		for(i = 0;i<num;i++)
		{
			data_p[i+num].xangle = anglex_orignal + 
				cos(PI*(float)(i+1)/(float)num - PI/2)*(anglex_dithering_value-anglex_orignal);  
			data_p[2*num-i-1].yangle = (sin(PI*(float)(i+1)/(float)num - PI/2)+1)/2*(y_end-y_start);
			data_p[i+num].zangle = 0;
			data_p[i+num].dist = -3;
			data_p[i+num].factor = scale_orignal + 
				cos((float)(i+1)/(float)num*PI - PI/2)*(scale_dithering_value-scale_orignal);
		}
	}
}

ALLEGRO_BITMAP* Tube3DTransition::GetFirst(int w, int h)
{
	if(bitmap_first == NULL)
	{    
		//create bitmap
        bitmap_first = al_create_bitmap(w,h);
        
        int t_w = 0,t_h = 0;
        al_get_opengl_texture_size(this->bitmap_first, &t_w, &t_h);
        if(t_w && t_h)
        {
            tex_coords_1st.x1 = (GLfloat)w/(GLfloat)t_w;
            tex_coords_1st.y1 = (GLfloat)h/(GLfloat)t_h;
        }                
	}

	return this->bitmap_first;
}

ALLEGRO_BITMAP* Tube3DTransition::GetSecond(int w, int h)
{
	if(bitmap_second == NULL)
	{      
		//create bitmap
        bitmap_second = al_create_bitmap(w,h);
        
        int t_w = 0,t_h = 0;
        al_get_opengl_texture_size(this->bitmap_second, &t_w, &t_h);
        if(t_w && t_h)
        {
            tex_coords_2nd.x1 = (GLfloat)w/(GLfloat)t_w;
            tex_coords_2nd.y1 = (GLfloat)h/(GLfloat)t_h;
        }                
	}

	return this->bitmap_second;
}

void Tube3DTransition::UpdateFirst(Drawable* p,int offsetx,int offsety, double zoom)
{
    ALLEGRO_BITMAP *target = GetFirst(width,height);
    
    if(target == NULL)
        return;
    
    al_set_target_bitmap(target);
    al_clear_to_color(al_map_rgba_f(0, 0, 0, 0));
    
    p->Render(offsetx,offsety,zoom);
    
    al_set_target_backbuffer(al_get_current_display());
    
    return;
}

void Tube3DTransition::UpdateSecond(Drawable* p,int offsetx,int offsety, double zoom)
{
    ALLEGRO_BITMAP *target = GetSecond(width,height);
    
    if(target == NULL)
        return;
    
    al_set_target_bitmap(target);
    al_clear_to_color(al_map_rgba_f(0, 0, 0, 0));
    
    p->Render(offsetx,offsety,zoom);
    
    al_set_target_backbuffer(al_get_current_display());    
    return;
}

void Tube3DTransition::MovingCamera()
{
	int i = current_frame % frame_num;

	if(current_frame >= frame_num)
		i = frame_num - 1;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//glFrustum(-1.0, 1.0, -1.0, 1.0, 1.0, 40.0);
#ifndef ALLEGRO_IPHONE
	glOrtho(-1.0, 1.0, -1.0, 1.0, 1.0, 40.0);
#else
    glOrthof(-1.0, 1.0, -1.0, 1.0, 1.0, 40.0);
#endif
	glTranslatef(0, 0, data_p[i].dist);
	glRotatef(data_p[i].xangle, 1, 0, 0);
	glRotatef(data_p[i].yangle, 0, 1, 0);
	glRotatef(data_p[i].zangle, 0, 0, 1);
	glScalef(data_p[i].factor,data_p[i].factor,data_p[i].factor);
	glMatrixMode(GL_MODELVIEW);
}

void Tube3DTransition::Prepare()
{
    //don't need to delete opengl es texture, allegro manage it
    
	if(bitmap_first)
    {
		tex_first = al_get_opengl_texture(this->bitmap_first);
    }
	if(bitmap_second)
    {
		tex_second = al_get_opengl_texture(this->bitmap_second);
    }

	GLuint f = this->tex_first;
	GLuint s = this->tex_second;
	GLuint o = this->tex_other;

	if(transition_mode == TO_RIGHT)
	{
		tex_array[FRONT]	= f;
		tex_array[RIGHT]	= o;
		tex_array[BACK]		= o;
		tex_array[LEFT]		= s;
		tex_array[TOP]		= o;
		tex_array[BOTTOM]	= o;
        
		tex_coords[FRONT]	= tex_coords_1st;
		tex_coords[LEFT]	= tex_coords_2nd;
		tex_coords[RIGHT]	= tex_coords_other;
		tex_coords[BACK]    = tex_coords_other;
		tex_coords[TOP]		= tex_coords_other;
		tex_coords[BOTTOM]	= tex_coords_other;
	}
	else if(transition_mode == TO_LEFT)
	{
		tex_array[FRONT]	= f;
		tex_array[RIGHT]	= s;
		tex_array[BACK]		= o;
		tex_array[LEFT]		= o;
		tex_array[TOP]		= o;
		tex_array[BOTTOM]	= o;
		tex_coords[FRONT]	= tex_coords_1st;
		tex_coords[RIGHT]	= tex_coords_2nd;
		tex_coords[LEFT]	= tex_coords_other;
		tex_coords[BACK]    = tex_coords_other;
		tex_coords[TOP]		= tex_coords_other;
		tex_coords[BOTTOM]	= tex_coords_other;        
	}
	else if(transition_mode == TO_LEFT_FRONT)
	{
		tex_array[FRONT]	= f;
		tex_array[RIGHT]	= s;
		tex_array[BACK]		= o;
		tex_array[LEFT]		= o;
		tex_array[TOP]		= o;
		tex_array[BOTTOM]	= o;
		tex_coords[FRONT]	= tex_coords_1st;
		tex_coords[RIGHT]	= tex_coords_2nd;
		tex_coords[LEFT]	= tex_coords_other;
		tex_coords[BACK]    = tex_coords_other;
		tex_coords[TOP]		= tex_coords_other;
		tex_coords[BOTTOM]	= tex_coords_other;        
	}
	else if(transition_mode == TO_RIGHT_FRONT)
	{
		tex_array[FRONT]	= f;
		tex_array[RIGHT]	= o;
		tex_array[BACK]		= o;
		tex_array[LEFT]		= s;
		tex_array[TOP]		= o;
		tex_array[BOTTOM]	= o;
		tex_coords[FRONT]	= tex_coords_1st;
		tex_coords[LEFT]	= tex_coords_2nd;    
		tex_coords[RIGHT]	= tex_coords_other;
		tex_coords[BACK]    = tex_coords_other;
		tex_coords[TOP]		= tex_coords_other;
		tex_coords[BOTTOM]	= tex_coords_other;        
	}
	else
	{
		tex_array[FRONT]	= f;
		tex_array[RIGHT]	= s;
		tex_array[BACK]		= o;
		tex_array[LEFT]		= o;
		tex_array[TOP]		= o;
		tex_array[BOTTOM]	= o;
		tex_coords[FRONT]	= tex_coords_1st;
		tex_coords[RIGHT]	= tex_coords_2nd;     
		tex_coords[LEFT]	= tex_coords_other;
		tex_coords[BACK]    = tex_coords_other;
		tex_coords[TOP]		= tex_coords_other;
		tex_coords[BOTTOM]	= tex_coords_other;        
	}

	current_frame = 0;

	//save state
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
#ifndef ALLEGRO_IPHONE
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);
#endif
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	//glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

void Tube3DTransition::Finish()
{
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);

	glDisable(GL_CULL_FACE);    
    
#ifndef ALLEGRO_IPHONE
	glPopClientAttrib();
	glPopAttrib();
#endif
}

bool Tube3DTransition::IsFinished()
{
	return current_frame >= frame_num;
}

void Tube3DTransition::Render(int x, int y, double zoom)
{
    (void)x;
    (void)y;
    (void)zoom;
        
	if(intermediate_num && current_frame == this->frame_num / 2)
	{
		intermediate_num--;
	}
	else
	{
		MovingCamera();
		this->current_frame++;
	}
    
    GLfloat texcoords[4][2];
    GLfloat vertices[4][3];
    GLubyte indices[4]={0, 1, 3, 2}; /* QUAD to TRIANGLE_STRIP conversion; */

    /* Clear The Screen And The Depth Buffer */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /* Move Into The Screen 5 Units */
    glLoadIdentity();

    /* Select Our Texture */
	glEnable(GL_TEXTURE_2D);

    /* Set pointers to vertices and texcoords */
    glVertexPointer(3, GL_FLOAT, 0, vertices);
    glTexCoordPointer(2, GL_FLOAT, 0, texcoords);

    /* Enable vertices and texcoords arrays */
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    /* Top Face */
    glBindTexture(GL_TEXTURE_2D, tex_array[TOP]);
    texcoords[0][0]=tex_coords[TOP].x1; texcoords[0][1]=tex_coords[TOP].y1;
    vertices[0][0]=-1.0f; vertices[0][1]=1.0f; vertices[0][2]=-1.0f;
    texcoords[1][0]=tex_coords[TOP].x1; texcoords[1][1]=0.0f;
    vertices[1][0]=-1.0f; vertices[1][1]=1.0f; vertices[1][2]=1.0f;
    texcoords[2][0]=0.0f; texcoords[2][1]=0.0f;
    vertices[2][0]=1.0f;  vertices[2][1]=1.0f; vertices[2][2]=1.0f;
    texcoords[3][0]=0.0f; texcoords[3][1]=tex_coords[TOP].y1;
    vertices[3][0]=1.0f;  vertices[3][1]=1.0f; vertices[3][2]=-1.0f;

    /* Draw one textured plane using two stripped triangles */
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, indices);

    /* Bottom Face */
    glBindTexture(GL_TEXTURE_2D, tex_array[BOTTOM]);
    texcoords[0][0]=0.0f; texcoords[0][1]=tex_coords[BOTTOM].y1;
    vertices[0][0]=-1.0f; vertices[0][1]=-1.0f; vertices[0][2]=-1.0f;
    texcoords[1][0]=tex_coords[BOTTOM].x1; texcoords[1][1]=tex_coords[BOTTOM].y1;
    vertices[1][0]=1.0f;  vertices[1][1]=-1.0f; vertices[1][2]=-1.0f;
    texcoords[2][0]=tex_coords[BOTTOM].x1; texcoords[2][1]=0.0f;
    vertices[2][0]=1.0f;  vertices[2][1]=-1.0f; vertices[2][2]=1.0f;
    texcoords[3][0]=0.0f; texcoords[3][1]=0.0f;
    vertices[3][0]=-1.0f; vertices[3][1]=-1.0f; vertices[3][2]=1.0f;

    /* Draw one textured plane using two stripped triangles */
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, indices);

    /* Front Face */
    glBindTexture(GL_TEXTURE_2D, tex_array[FRONT]);

    texcoords[0][0]=0.0f; texcoords[0][1]=0.0f;
    vertices[0][0]=-1.0f; vertices[0][1]=-1.0f; vertices[0][2]=1.0f;
    texcoords[1][0]=tex_coords[FRONT].x1; texcoords[1][1]=0.0f;
    vertices[1][0]=1.0f;  vertices[1][1]=-1.0f; vertices[1][2]=1.0f;
    texcoords[2][0]=tex_coords[FRONT].x1; texcoords[2][1]=tex_coords[FRONT].y1;
    vertices[2][0]=1.0f;  vertices[2][1]=1.0f; vertices[2][2]=1.0f;
    texcoords[3][0]=0.0f; texcoords[3][1]=tex_coords[FRONT].y1;
    vertices[3][0]=-1.0f; vertices[3][1]=1.0f; vertices[3][2]=1.0f;

    /* Draw one textured plane using two stripped triangles */
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, indices);

    /* Right face */
    glBindTexture(GL_TEXTURE_2D, tex_array[RIGHT]);

    texcoords[0][0]=0.0f; texcoords[0][1]=0.0f;
    vertices[0][0]=1.0f;  vertices[0][1]=-1.0f; vertices[0][2]=1.0f;
    texcoords[1][0]=tex_coords[RIGHT].x1; texcoords[1][1]=0.0f;
    vertices[1][0]=1.0f;  vertices[1][1]=-1.0f; vertices[1][2]=-1.0f;
    texcoords[2][0]=tex_coords[RIGHT].x1; texcoords[2][1]=tex_coords[RIGHT].y1;
    vertices[2][0]=1.0f;  vertices[2][1]=1.0f; vertices[2][2]=-1.0f;
    texcoords[3][0]=0.0f; texcoords[3][1]=tex_coords[RIGHT].y1;
    vertices[3][0]=1.0f;  vertices[3][1]=1.0f; vertices[3][2]=1.0f;

    /* Draw one textured plane using two stripped triangles */
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, indices);

    /* Back Face */
    glBindTexture(GL_TEXTURE_2D, tex_array[BACK]);

    /* Normal Pointing Away From Viewer */
    texcoords[0][0]=0.0f; texcoords[0][1]=0.0f;
    vertices[0][0]=1.0f; vertices[0][1]=-1.0f; vertices[0][2]=-1.0f;
    texcoords[1][0]=1.0f; texcoords[1][1]=0.0f;
    vertices[1][0]=-1.0f; vertices[1][1]=-1.0f; vertices[1][2]=-1.0f;
    texcoords[2][0]=1.0f; texcoords[2][1]=1.0f;
    vertices[2][0]=-1.0f;  vertices[2][1]=1.0f; vertices[2][2]=-1.0f;
    texcoords[3][0]=0.0f; texcoords[3][1]=1.0f;
    vertices[3][0]=1.0f; vertices[3][1]=1.0f; vertices[3][2]=-1.0f;

    /* Draw one textured plane using two stripped triangles */
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, indices);

    /* Left Face*/
    glBindTexture(GL_TEXTURE_2D, tex_array[LEFT]);

    texcoords[0][0]=0.0f; texcoords[0][1]=0.0f;
    vertices[0][0]=-1.0f; vertices[0][1]=-1.0f; vertices[0][2]=-1.0f;
    texcoords[1][0]=tex_coords[LEFT].x1; texcoords[1][1]=0.0f;
    vertices[1][0]=-1.0f; vertices[1][1]=-1.0f; vertices[1][2]=1.0f;
    texcoords[2][0]=tex_coords[LEFT].x1;texcoords[2][1]=tex_coords[LEFT].y1;
    vertices[2][0]=-1.0f; vertices[2][1]=1.0f; vertices[2][2]=1.0f;
    texcoords[3][0]=0.0f; texcoords[3][1]=tex_coords[LEFT].y1;
    vertices[3][0]=-1.0f; vertices[3][1]=1.0f; vertices[3][2]=-1.0f;

    /* Draw one textured plane using two stripped triangles */
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, indices);

    /* Disable texcoords and vertices arrays */
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);

    /* Flush all drawings */
    glFinish();
}

//void Tube3DTransition::Render(int x, int y, double zoom)
//{
//    (void)x;
//    (void)y;
//    (void)zoom;
//    
//	if(intermediate_num && current_frame == this->frame_num / 2)
//	{
//		intermediate_num--;
//	}
//	else
//	{
//		MovingCamera();
//		this->current_frame++;
//	}
//    
//    GLfloat tex_x_first[2] = {0};
//    GLfloat tex_y_first[2] = {0};
//    int w = 0,h = 0;
//    al_get_opengl_texture_size(this->bitmap_first, &w, &h);
//    if(w == 0 || h == 0)
//    {
//        tex_x_first[1] = 1;
//        tex_y_first[1] = 1;
//    }
//    else
//    {
//        tex_x_first[1] = 320.0/(GLfloat)w;
//        tex_y_first[1] = 480.0/(GLfloat)h;
//    }
//    
//    GLfloat texcoords[4][2];
//    GLfloat vertices[4][3];
//    GLubyte indices[4]={0, 1, 3, 2}; /* QUAD to TRIANGLE_STRIP conversion; */
//    
//    /* Clear The Screen And The Depth Buffer */
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//    
//    /* Move Into The Screen 5 Units */
//    glLoadIdentity();
//    
//    /* Select Our Texture */
//	glEnable(GL_TEXTURE_2D);
//    
//    /* Set pointers to vertices and texcoords */
//    glVertexPointer(3, GL_FLOAT, 0, vertices);
//    glTexCoordPointer(2, GL_FLOAT, 0, texcoords);
//    
//    /* Enable vertices and texcoords arrays */
//    glEnableClientState(GL_VERTEX_ARRAY);
//    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
//    
//    /* Top Face */
//    glBindTexture(GL_TEXTURE_2D, tex_array[TOP]);
//    texcoords[0][0]=1.0f; texcoords[0][1]=1.0f;
//    vertices[0][0]=-1.0f; vertices[0][1]=1.0f; vertices[0][2]=-1.0f;
//    texcoords[1][0]=1.0f; texcoords[1][1]=0.0f;
//    vertices[1][0]=-1.0f; vertices[1][1]=1.0f; vertices[1][2]=1.0f;
//    texcoords[2][0]=0.0f; texcoords[2][1]=0.0f;
//    vertices[2][0]=1.0f;  vertices[2][1]=1.0f; vertices[2][2]=1.0f;
//    texcoords[3][0]=0.0f; texcoords[3][1]=1.0f;
//    vertices[3][0]=1.0f;  vertices[3][1]=1.0f; vertices[3][2]=-1.0f;
//    
//    /* Draw one textured plane using two stripped triangles */
//    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, indices);
//    
//    /* Bottom Face */
//    glBindTexture(GL_TEXTURE_2D, tex_array[BOTTOM]);
//    texcoords[0][0]=0.0f; texcoords[0][1]=1.0f;
//    vertices[0][0]=-1.0f; vertices[0][1]=-1.0f; vertices[0][2]=-1.0f;
//    texcoords[1][0]=1.0f; texcoords[1][1]=1.0f;
//    vertices[1][0]=1.0f;  vertices[1][1]=-1.0f; vertices[1][2]=-1.0f;
//    texcoords[2][0]=1.0f; texcoords[2][1]=0.0f;
//    vertices[2][0]=1.0f;  vertices[2][1]=-1.0f; vertices[2][2]=1.0f;
//    texcoords[3][0]=0.0f; texcoords[3][1]=0.0f;
//    vertices[3][0]=-1.0f; vertices[3][1]=-1.0f; vertices[3][2]=1.0f;
//    
//    /* Draw one textured plane using two stripped triangles */
//    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, indices);
//    
//    /* Front Face */
//    glBindTexture(GL_TEXTURE_2D, tex_array[FRONT]);
//    
//    texcoords[0][0]=0.0f; texcoords[0][1]=0.0f;
//    vertices[0][0]=-1.0f; vertices[0][1]=-1.0f; vertices[0][2]=1.0f;
//    texcoords[1][0]=tex_x_first[1]; texcoords[1][1]=0.0f;
//    vertices[1][0]=1.0f;  vertices[1][1]=-1.0f; vertices[1][2]=1.0f;
//    texcoords[2][0]=tex_x_first[1] ; texcoords[2][1]=tex_y_first[1] ;
//    vertices[2][0]=1.0f;  vertices[2][1]=1.0f; vertices[2][2]=1.0f;
//    texcoords[3][0]=0.0f; texcoords[3][1]=tex_y_first[1] ;
//    vertices[3][0]=-1.0f; vertices[3][1]=1.0f; vertices[3][2]=1.0f;
//    
//    /* Draw one textured plane using two stripped triangles */
//    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, indices);
//    
//    /* Right face */
//    glBindTexture(GL_TEXTURE_2D, tex_array[RIGHT]);
//    
//    texcoords[0][0]=0.0f; texcoords[0][1]=0.0f;
//    vertices[0][0]=1.0f;  vertices[0][1]=-1.0f; vertices[0][2]=1.0f;
//    texcoords[1][0]=tex_x_first[1] ; texcoords[1][1]=0.0f;
//    vertices[1][0]=1.0f;  vertices[1][1]=-1.0f; vertices[1][2]=-1.0f;
//    texcoords[2][0]=tex_x_first[1] ; texcoords[2][1]=tex_y_first[1] ;
//    vertices[2][0]=1.0f;  vertices[2][1]=1.0f; vertices[2][2]=-1.0f;
//    texcoords[3][0]=0.0f; texcoords[3][1]=tex_y_first[1] ;
//    vertices[3][0]=1.0f;  vertices[3][1]=1.0f; vertices[3][2]=1.0f;
//    
//    /* Draw one textured plane using two stripped triangles */
//    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, indices);
//    
//    /* Back Face */
//    glBindTexture(GL_TEXTURE_2D, tex_array[BACK]);
//    
//    /* Normal Pointing Away From Viewer */
//    texcoords[0][0]=0.0f; texcoords[0][1]=0.0f;
//    vertices[0][0]=1.0f; vertices[0][1]=-1.0f; vertices[0][2]=-1.0f;
//    texcoords[1][0]=1.0f; texcoords[1][1]=0.0f;
//    vertices[1][0]=-1.0f; vertices[1][1]=-1.0f; vertices[1][2]=-1.0f;
//    texcoords[2][0]=1.0f; texcoords[2][1]=1.0f;
//    vertices[2][0]=-1.0f;  vertices[2][1]=1.0f; vertices[2][2]=-1.0f;
//    texcoords[3][0]=0.0f; texcoords[3][1]=1.0f;
//    vertices[3][0]=1.0f; vertices[3][1]=1.0f; vertices[3][2]=-1.0f;
//    
//    /* Draw one textured plane using two stripped triangles */
//    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, indices);
//    
//    /* Left Face*/
//    glBindTexture(GL_TEXTURE_2D, tex_array[LEFT]);
//    
//    texcoords[0][0]=0.0f; texcoords[0][1]=0.0f;
//    vertices[0][0]=-1.0f; vertices[0][1]=-1.0f; vertices[0][2]=-1.0f;
//    texcoords[1][0]=tex_x_first[1]; texcoords[1][1]=0.0f;
//    vertices[1][0]=-1.0f; vertices[1][1]=-1.0f; vertices[1][2]=1.0f;
//    texcoords[2][0]=tex_x_first[1]; texcoords[2][1]=tex_y_first[1];
//    vertices[2][0]=-1.0f; vertices[2][1]=1.0f; vertices[2][2]=1.0f;
//    texcoords[3][0]=0.0f; texcoords[3][1]=tex_y_first[1];
//    vertices[3][0]=-1.0f; vertices[3][1]=1.0f; vertices[3][2]=-1.0f;
//    
//    /* Draw one textured plane using two stripped triangles */
//    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, indices);
//    
//    /* Disable texcoords and vertices arrays */
//    glDisableClientState(GL_NORMAL_ARRAY);
//    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
//    glDisableClientState(GL_VERTEX_ARRAY);
//    
//    /* Flush all drawings */
//    glFinish();
//}







