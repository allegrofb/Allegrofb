/* An example program showing how to set and use a depth buffer with an OpenGL
 * display. Use arrow keys to rotate, PgUp/PgDown to move closer/farther away.
 */


//#include "allegro5/allegro.h"
//#include "allegro5/allegro_image.h"
//#include "allegro5/allegro_opengl.h"
//#include "allegro5/allegro_font.h"
//
//#include "common.c"
//
//struct camera {
//   double xangle, yangle, zangle;
//   double dist;
//};
//
//struct camera camera = {
//   0.0, 0.0, 0.0,
//   20.0
//};
//
//double angle_speed = 5.0;
//double dist_speed = 1.0;
//
//GLuint tex;
//ALLEGRO_BITMAP *bmp;
//bool key[ALLEGRO_KEY_MAX];
//
//static void set_camera_position(void)
//{
//   glMatrixMode(GL_PROJECTION);
//   glLoadIdentity();
//   glFrustum(-1.0, 1.0, -1.0, 1.0, 1.0, 40.0);
//   glTranslatef(0, 0, -camera.dist);
//   glRotatef(camera.xangle, 1, 0, 0);
//   glRotatef(camera.yangle, 0, 1, 0);
//   glRotatef(camera.zangle, 0, 0, 1);
//   glMatrixMode(GL_MODELVIEW);
//}
//
//
//
//static void keyboard(void)
//{
//   if(key[ALLEGRO_KEY_LEFT])  camera.yangle += angle_speed;
//   if(key[ALLEGRO_KEY_RIGHT]) camera.yangle -= angle_speed;
//
//   if(key[ALLEGRO_KEY_UP])   camera.xangle += angle_speed;
//   if(key[ALLEGRO_KEY_DOWN]) camera.xangle -= angle_speed;
//
//   if(key[ALLEGRO_KEY_PGUP]) camera.dist -= dist_speed;
//   if(key[ALLEGRO_KEY_PGDN]) camera.dist += dist_speed;
//}
//
//
//
//static void draw(void)
//{
//   // Clear the RGB buffer and the depth buffer
//   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//   // Set the modelview matrix to be the identity matrix
//   glLoadIdentity();
//   // Translate and rotate the object
//   glTranslatef(-2.5, 0.0, 0.0);
//   glRotatef(-30, 1.0, 0.0, 0.0);
//   glRotatef(30, 0.0, 1.0, 0.0);
//   glRotatef(30, 0.0, 0.0, 1.0);
//
//   glColor3f(1.0, 0.0, 1.0);
//
//   // Draw the sides of the three-sided pyramid
//   glEnable(GL_TEXTURE_2D);
//   glBindTexture(GL_TEXTURE_2D, tex);
//   glBegin(GL_TRIANGLE_FAN);
//         glTexCoord2f(0, 0); glVertex3d(0, 4, 0);
//         glTexCoord2f(1, 0); glVertex3d(0, -4, -4);
//         glTexCoord2f(1, 1); glVertex3d(-4, -4, 4);
//         glTexCoord2f(0, 1); glVertex3d(4, -4, 4);
//         glTexCoord2f(1, 0); glVertex3d(0, -4, -4);
//   glEnd();
//
//   glColor3f(0.0, 1.0, 1.0);
//
//   // Draw the base of the pyramid
//   glBegin(GL_TRIANGLES);
//         glTexCoord2f(1, 0); glVertex3d(0, -4, -4);
//         glTexCoord2f(0, 1); glVertex3d(4, -4, 4);
//         glTexCoord2f(1, 1); glVertex3d(-4, -4, 4);
//   glEnd();
//
//
//   glLoadIdentity();
//   glTranslatef(2.5, 0.0, 0.0);
//   glRotatef(45, 1.0, 0.0, 0.0);
//   glRotatef(45, 0.0, 1.0, 0.0);
//   glRotatef(45, 0.0, 0.0, 1.0);
//
//   glColor3f(0.0, 1.0, 0.0);
//
//   glDisable(GL_TEXTURE_2D);
//   // Draw the sides of the cube
//   glBegin(GL_QUAD_STRIP);
//      glVertex3d(3, 3, -3);
//      glVertex3d(3, -3, -3);
//      glVertex3d(-3, 3, -3);
//      glVertex3d(-3, -3, -3);
//      glVertex3d(-3, 3, 3);
//      glVertex3d(-3, -3, 3);
//      glVertex3d(3, 3, 3);
//      glVertex3d(3, -3, 3);
//      glVertex3d(3, 3, -3);
//      glVertex3d(3, -3, -3);
//   glEnd();
//
//   glColor3f(0.0, 0.0, 1.0);
//
//   // Draw the top of the cube
//   glBegin(GL_QUADS);
//      glVertex3d(-3, -3, -3);
//      glVertex3d(3, -3, -3);
//      glVertex3d(3, -3, 3);
//      glVertex3d(-3, -3, 3);
//   glEnd();
//
//   /* Bottom is texture-mapped */
//   glEnable(GL_TEXTURE_2D);
//   glBindTexture(GL_TEXTURE_2D, tex);
//   glBegin(GL_QUADS);
//      glTexCoord2f(0, 0); glVertex3d(-3, 3, -3);
//      glTexCoord2f(1, 0); glVertex3d(-3, 3, 3);
//      glTexCoord2f(1, 1); glVertex3d(3, 3, 3);
//      glTexCoord2f(0, 1); glVertex3d(3, 3, -3);
//   glEnd();
//}
//
//
//
//static void setup_textures(ALLEGRO_DISPLAY *display)
//{
//   ALLEGRO_BITMAP *tmp_bmp;
//   ALLEGRO_FONT *font;
//   int w, h, depth;
//
//   font = al_load_font("data/fixed_font.tga", 0, 0);
//   if(!font) {
//      abort_example("Error loading `data/fixed_font.tga'\n");
//      exit(1);
//   }
//
//   tmp_bmp = al_load_bitmap("data/mysha.pcx");
//   if(!tmp_bmp) {
//      abort_example("Error loading `data/mysha.pcx'\n");
//      exit(1);
//   }
//   w = 128;
//   h = 128;
//   bmp = al_create_bitmap(w, h);
//   al_set_target_bitmap(bmp);
//   al_draw_scaled_bitmap(tmp_bmp,
//                         0, 0, al_get_bitmap_width(bmp), al_get_bitmap_height(bmp),
//                         0, 0, w, h, 0);
//
//   depth = al_get_display_option(display, ALLEGRO_DEPTH_SIZE);
//   if (!depth)
//      al_draw_textf(font, al_map_rgb(255, 0, 0), 0, 5, 0, "No Z-buffer!");
//   else
//      al_draw_textf(font, al_map_rgb(255, 0, 0), 0, 5, 0, "Z-buffer: %i bits", depth);
//   al_set_target_backbuffer(display);
//   al_destroy_bitmap(tmp_bmp);
//   al_destroy_font(font);
//
//   glEnable(GL_TEXTURE_2D);
//   glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
//
//   tex = al_get_opengl_texture(bmp);
//}
//
//
//
//int main(void)
//{
//   ALLEGRO_DISPLAY *display;
//   ALLEGRO_EVENT_QUEUE *queue;
//   ALLEGRO_TIMER *timer;
//   ALLEGRO_EVENT event;
//
//   if(!al_init()) {
//      abort_example("Could not init Allegro.\n");
//      return 1;
//   }
//
//   al_init_image_addon();
//   al_init_font_addon();
//   al_install_keyboard();
//
//   al_set_new_display_flags(ALLEGRO_OPENGL);
//   al_set_new_display_option(ALLEGRO_DEPTH_SIZE, 16, ALLEGRO_SUGGEST);
//   display = al_create_display(640, 480);
//   if(!display) {
//      abort_example("Could not create display.\n");
//   }
//
//   timer = al_create_timer(1. / 60.);
//
//   queue = al_create_event_queue();
//   al_register_event_source(queue,al_get_keyboard_event_source());
//   al_register_event_source(queue,al_get_display_event_source(display));
//   al_register_event_source(queue,al_get_timer_event_source(timer));
//
//   glEnable(GL_DEPTH_TEST);
//   glDisable(GL_CULL_FACE);
//
//   setup_textures(display);
//   al_start_timer(timer);
//
//   while(true) {
//      al_wait_for_event(queue, &event);
//      switch(event.type) {
//         case ALLEGRO_EVENT_DISPLAY_CLOSE:
//            goto done;
//
//         case ALLEGRO_EVENT_KEY_DOWN:
//            if(event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
//               goto done;
//            key[event.keyboard.keycode] = true;
//            break;
//         
//         case ALLEGRO_EVENT_KEY_UP:
//            key[event.keyboard.keycode] = false;
//            break;
//
//         case ALLEGRO_EVENT_TIMER:
//            keyboard();
//            if(al_is_event_queue_empty(queue)) {
//               set_camera_position();
//               draw();
//               al_flip_display();
//            }
//            break;
//      }
//   }
//
//done:
//   al_destroy_bitmap(bmp);
//
//   return 0;
//}
//


//----------------------------------------------------------------------------

#include "allegro5/allegro.h"
#include "allegro5/allegro_image.h"
#include "allegro5/allegro_opengl.h"
#include "allegro5/allegro_font.h"

#include "common.c"

struct camera {
   double xangle, yangle, zangle;
   double dist;
   double factor;
};

struct camera camera = {
   0.0, 0.0, 0.0,
   3.0,
   1.0
};

float scale_orignal = 1;
float scale_dithering_value = 0.85;

float angley_start = 0;
float angley_end = -90;

float anglex_orignal = 0;
float anglex_dithering_value = 6;


float transition_time = 0.8;
float frame_rate = 30;
int frame_num = 0;
struct camera *data_p = NULL;

static void calculate_frame()
{
	int i = 0;
	frame_num = frame_rate*transition_time;

	if(data_p)
		free(data_p);

	data_p = malloc(sizeof(camera)*frame_num);

	for(i = 0;i<frame_num;i++)
	{
		//linear

		//if(i > frame_num / 2)
		//	data_p[i].xangle = (anglex_end-anglex_start)/frame_num*(frame_num-i-1)*2;
		//else
		//	data_p[i].xangle = (anglex_end-anglex_start)/frame_num*(i+1)*2;

		//data_p[i].yangle = (angley_end-angley_start)/frame_num*(i+1);
		//data_p[i].zangle = 0;
		//data_p[i].dist = 3;

		//if(i > frame_num / 2)
		//	data_p[i].factor = scale_max - (scale_max-scale_min)/frame_num*(frame_num-i-1)*2;
		//else
		//	data_p[i].factor = scale_max - (scale_max-scale_min)/frame_num*(i+1)*2;


#define PI 3.14159265

		data_p[i].xangle = anglex_orignal + 
				cos(PI*(float)(i+1)/(float)frame_num - PI/2)*(anglex_dithering_value-anglex_orignal);  
		data_p[i].yangle = (sin(PI*(float)(i+1)/(float)frame_num - PI/2)+1)/2*(angley_end-angley_start);
		data_p[i].zangle = 0;
		data_p[i].dist = 3;
		data_p[i].factor = scale_orignal + 
			cos((float)(i+1)/(float)frame_num*PI - PI/2)*(scale_dithering_value-scale_orignal);
	}
}
static void render_frame(int frame)
{
	int i = frame % frame_num;

	if(frame >= frame_num)
		i = frame_num - 1;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//glFrustum(-1.0, 1.0, -1.0, 1.0, 1.0, 40.0);
	glOrtho(-1.0, 1.0, -1.0, 1.0, 1.0, 40.0);
	//glTranslatef(0, 0, -camera.dist);
	glTranslatef(0, 0, -3);			//does it means that z point inside
	glRotatef(data_p[i].xangle, 1, 0, 0);
	glRotatef(data_p[i].yangle, 0, 1, 0);
	glRotatef(data_p[i].zangle, 0, 0, 1);
	glScalef(data_p[i].factor,data_p[i].factor,data_p[i].factor);
	glMatrixMode(GL_MODELVIEW);
}




double angle_speed = 0.5;
//double angle_speed = 5.0;
//double dist_speed = 1.0;
double dist_speed = 0.01;

GLuint tex_array[4];
ALLEGRO_BITMAP *bmp;
bool key[ALLEGRO_KEY_MAX];

static void set_camera_position(void)
{
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   //glFrustum(-1.0, 1.0, -1.0, 1.0, 1.0, 40.0);
   glOrtho(-1.0, 1.0, -1.0, 1.0, 1.0, 40.0);
   glTranslatef(0, 0, -camera.dist);
   glRotatef(camera.xangle, 1, 0, 0);
   glRotatef(camera.yangle, 0, 1, 0);
   glRotatef(camera.zangle, 0, 0, 1);
   glScalef(camera.factor,camera.factor,camera.factor);
   glMatrixMode(GL_MODELVIEW);
   //glLoadIdentity();
   //glViewport(0,0,320*camera.factor,480*camera.factor);
}

static void keyboard(void)
{
   if(key[ALLEGRO_KEY_LEFT])  camera.yangle += angle_speed;
   if(key[ALLEGRO_KEY_RIGHT]) camera.yangle -= angle_speed;

   if(key[ALLEGRO_KEY_UP])   camera.xangle += angle_speed;
   if(key[ALLEGRO_KEY_DOWN]) camera.xangle -= angle_speed;

   if(key[ALLEGRO_KEY_PGUP]) camera.dist -= dist_speed;
   if(key[ALLEGRO_KEY_PGDN]) camera.dist += dist_speed;
}

static void draw(void)
{
    /* These are to calculate our fps */
    static GLint T0=0;
    static GLint Frames=0;
    GLfloat texcoords[4][2];
    GLfloat vertices[4][3];
    GLubyte indices[4]={0, 1, 3, 2}; /* QUAD to TRIANGLE_STRIP conversion; */

    /* Clear The Screen And The Depth Buffer */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /* Move Into The Screen 5 Units */
    glLoadIdentity();
	//glScalef (1., 1., -1.);		//hyjiang for test

    /* Select Our Texture */
	glEnable(GL_TEXTURE_2D);

    /* Set pointers to vertices and texcoords */
    glVertexPointer(3, GL_FLOAT, 0, vertices);
    glTexCoordPointer(2, GL_FLOAT, 0, texcoords);

    /* Enable vertices and texcoords arrays */
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    /* Top Face */
    glBindTexture(GL_TEXTURE_2D, 0);
    texcoords[0][0]=1.0f; texcoords[0][1]=1.0f;
    vertices[0][0]=-1.0f; vertices[0][1]=1.0f; vertices[0][2]=-1.0f;
    texcoords[1][0]=1.0f; texcoords[1][1]=0.0f;
    vertices[1][0]=-1.0f; vertices[1][1]=1.0f; vertices[1][2]=1.0f;
    texcoords[2][0]=0.0f; texcoords[2][1]=0.0f;
    vertices[2][0]=1.0f;  vertices[2][1]=1.0f; vertices[2][2]=1.0f;
    texcoords[3][0]=0.0f; texcoords[3][1]=1.0f;
    vertices[3][0]=1.0f;  vertices[3][1]=1.0f; vertices[3][2]=-1.0f;

    /* Draw one textured plane using two stripped triangles */
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, indices);

    /* Bottom Face */
    glBindTexture(GL_TEXTURE_2D, 0);
    texcoords[0][0]=0.0f; texcoords[0][1]=1.0f;
    vertices[0][0]=-1.0f; vertices[0][1]=-1.0f; vertices[0][2]=-1.0f;
    texcoords[1][0]=1.0f; texcoords[1][1]=1.0f;
    vertices[1][0]=1.0f;  vertices[1][1]=-1.0f; vertices[1][2]=-1.0f;
    texcoords[2][0]=1.0f; texcoords[2][1]=0.0f;
    vertices[2][0]=1.0f;  vertices[2][1]=-1.0f; vertices[2][2]=1.0f;
    texcoords[3][0]=0.0f; texcoords[3][1]=0.0f;
    vertices[3][0]=-1.0f; vertices[3][1]=-1.0f; vertices[3][2]=1.0f;

    /* Draw one textured plane using two stripped triangles */
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, indices);

    /* Front Face */
    glBindTexture(GL_TEXTURE_2D, tex_array[0]);

    //texcoords[0][0]=1.0f; texcoords[0][1]=0.0f;
    //vertices[0][0]=-1.0f; vertices[0][1]=-1.0f; vertices[0][2]=1.0f;
    //texcoords[1][0]=0.0f; texcoords[1][1]=0.0f;
    //vertices[1][0]=1.0f;  vertices[1][1]=-1.0f; vertices[1][2]=1.0f;
    //texcoords[2][0]=0.0f; texcoords[2][1]=1.0f;
    //vertices[2][0]=1.0f;  vertices[2][1]=1.0f; vertices[2][2]=1.0f;
    //texcoords[3][0]=1.0f; texcoords[3][1]=1.0f;
    //vertices[3][0]=-1.0f; vertices[3][1]=1.0f; vertices[3][2]=1.0f;

	//		3 <- 2		y  1,1
	//		|	 |		|
	//		0 -> 1		x-->
	// texture coordinator and vertex coordinator should be match

    texcoords[0][0]=0.0f; texcoords[0][1]=0.0f;
    vertices[0][0]=-1.0f; vertices[0][1]=-1.0f; vertices[0][2]=1.0f;
    texcoords[1][0]=1.0f; texcoords[1][1]=0.0f;
    vertices[1][0]=1.0f;  vertices[1][1]=-1.0f; vertices[1][2]=1.0f;
    texcoords[2][0]=1.0f; texcoords[2][1]=1.0f;
    vertices[2][0]=1.0f;  vertices[2][1]=1.0f; vertices[2][2]=1.0f;
    texcoords[3][0]=0.0f; texcoords[3][1]=1.0f;
    vertices[3][0]=-1.0f; vertices[3][1]=1.0f; vertices[3][2]=1.0f;


    /* Draw one textured plane using two stripped triangles */
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, indices);

    /* Right face */
    glBindTexture(GL_TEXTURE_2D, tex_array[1]);

    //texcoords[0][0]=0.0f; texcoords[0][1]=0.0f;
    //vertices[0][0]=1.0f;  vertices[0][1]=-1.0f; vertices[0][2]=-1.0f;
    //texcoords[1][0]=0.0f; texcoords[1][1]=1.0f;
    //vertices[1][0]=1.0f;  vertices[1][1]=1.0f; vertices[1][2]=-1.0f;
    //texcoords[2][0]=1.0f; texcoords[2][1]=1.0f;
    //vertices[2][0]=1.0f;  vertices[2][1]=1.0f; vertices[2][2]=1.0f;
    //texcoords[3][0]=1.0f; texcoords[3][1]=0.0f;
    //vertices[3][0]=1.0f;  vertices[3][1]=-1.0f; vertices[3][2]=1.0f;

    texcoords[0][0]=0.0f; texcoords[0][1]=0.0f;
    vertices[0][0]=1.0f;  vertices[0][1]=-1.0f; vertices[0][2]=1.0f;
    texcoords[1][0]=1.0f; texcoords[1][1]=0.0f;
    vertices[1][0]=1.0f;  vertices[1][1]=-1.0f; vertices[1][2]=-1.0f;
    texcoords[2][0]=1.0f; texcoords[2][1]=1.0f;
    vertices[2][0]=1.0f;  vertices[2][1]=1.0f; vertices[2][2]=-1.0f;
    texcoords[3][0]=0.0f; texcoords[3][1]=1.0f;
    vertices[3][0]=1.0f;  vertices[3][1]=1.0f; vertices[3][2]=1.0f;

	/* Draw one textured plane using two stripped triangles */
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, indices);

    /* Back Face */
    glBindTexture(GL_TEXTURE_2D, tex_array[2]);

    /* Normal Pointing Away From Viewer */
    //texcoords[0][0]=0.0f; texcoords[0][1]=0.0f;
    //vertices[0][0]=-1.0f; vertices[0][1]=-1.0f; vertices[0][2]=-1.0f;
    //texcoords[1][0]=0.0f; texcoords[1][1]=1.0f;
    //vertices[1][0]=-1.0f; vertices[1][1]=1.0f; vertices[1][2]=-1.0f;
    //texcoords[2][0]=1.0f; texcoords[2][1]=1.0f;
    //vertices[2][0]=1.0f;  vertices[2][1]=1.0f; vertices[2][2]=-1.0f;
    //texcoords[3][0]=1.0f; texcoords[3][1]=0.0f;
    //vertices[3][0]=1.0f; vertices[3][1]=-1.0f; vertices[3][2]=-1.0f;

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
    glBindTexture(GL_TEXTURE_2D, tex_array[3]);

    texcoords[0][0]=0.0f; texcoords[0][1]=0.0f;
    vertices[0][0]=-1.0f; vertices[0][1]=-1.0f; vertices[0][2]=-1.0f;
    texcoords[1][0]=1.0f; texcoords[1][1]=0.0f;
    vertices[1][0]=-1.0f; vertices[1][1]=-1.0f; vertices[1][2]=1.0f;
    texcoords[2][0]=1.0f; texcoords[2][1]=1.0f;
    vertices[2][0]=-1.0f; vertices[2][1]=1.0f; vertices[2][2]=1.0f;
    texcoords[3][0]=0.0f; texcoords[3][1]=1.0f;
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
//{
//   // Clear the RGB buffer and the depth buffer
//   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//   // Set the modelview matrix to be the identity matrix
//   glLoadIdentity();
//   // Translate and rotate the object
//   //glTranslatef(-2.5, 0.0, 0.0);
//   //glRotatef(-30, 1.0, 0.0, 0.0);
//   //glRotatef(30, 0.0, 1.0, 0.0);
//   //glRotatef(30, 0.0, 0.0, 1.0);
//
//   glColor3f(1.0, 0.0, 1.0);
//
//   // Draw the sides of the cube
//   glBegin(GL_QUAD_STRIP);
//      glVertex3d(3, 3, -3);
//      glVertex3d(3, -3, -3);
//      glVertex3d(-3, 3, -3);
//      glVertex3d(-3, -3, -3);
//      glVertex3d(-3, 3, 3);
//      glVertex3d(-3, -3, 3);
//      glVertex3d(3, 3, 3);
//      glVertex3d(3, -3, 3);
//      glVertex3d(3, 3, -3);
//      glVertex3d(3, -3, -3);
//   glEnd();
//
//   // Draw the top of the cube
//   glColor3f(0.0, 0.0, 1.0);
//   glBegin(GL_QUADS);
//      glVertex3d(-3, -3, -3);
//      glVertex3d(3, -3, -3);
//      glVertex3d(3, -3, 3);
//      glVertex3d(-3, -3, 3);
//   glEnd();
//
//
//   // Draw the bottom of the cube
//   glColor3f(1.0, 0.0, 1.0);
//   glBegin(GL_QUADS);
//      glVertex3d(-3, 3, -3);
//      glVertex3d(-3, 3, 3);
//      glVertex3d(3, 3, 3);
//      glVertex3d(3, 3, -3);
//   glEnd();
//
//
//   /* Side is texture-mapped */
//   glEnable(GL_TEXTURE_2D);
//   glBindTexture(GL_TEXTURE_2D, tex_array[0]);
//   glBegin(GL_QUADS);
//      glTexCoord2f(0, 0); glVertex3d(-3, 3, -3);
//      glTexCoord2f(1, 0); glVertex3d(-3, 3, 3);
//      glTexCoord2f(1, 1); glVertex3d(3, 3, 3);
//      glTexCoord2f(0, 1); glVertex3d(3, 3, -3);
//   glEnd();
//}


//how to release texture ???????? glDeleteTextures (GLsizei n, const GLuint *textures);
static void setup_textures(ALLEGRO_DISPLAY *display)	
{
   ALLEGRO_BITMAP *tmp_bmp;

   //1st texture
   tmp_bmp = al_load_bitmap("data/1.jpg");
   if(!tmp_bmp) {
      abort_example("Error loading `data/1.jpg'\n");
      exit(1);
   }

   tex_array[0] = al_get_opengl_texture(tmp_bmp);

   //2nd texture
   tmp_bmp = al_load_bitmap("data/2.jpg");
   if(!tmp_bmp) {
      abort_example("Error loading `data/2.jpg'\n");
      exit(1);
   }

   tex_array[1] = al_get_opengl_texture(tmp_bmp);

   //3rd texture
   tmp_bmp = al_load_bitmap("data/3.jpg");
   if(!tmp_bmp) {
      abort_example("Error loading `data/3.jpg'\n");
      exit(1);
   }

   tex_array[2] = al_get_opengl_texture(tmp_bmp);

   //4th texture
   tmp_bmp = al_load_bitmap("data/4.jpg");
   if(!tmp_bmp) {
      abort_example("Error loading `data/4.jpg'\n");
      exit(1);
   }

   tex_array[3] = al_get_opengl_texture(tmp_bmp);
}



int main(void)
{
	ALLEGRO_DISPLAY *display;
	ALLEGRO_EVENT_QUEUE *queue;
	ALLEGRO_TIMER *timer;
	ALLEGRO_EVENT event;
	int frame = 0;
	
	if(!al_init()) {
		abort_example("Could not init Allegro.\n");
		return 1;
	}

	al_init_image_addon();
	al_init_font_addon();
	al_install_keyboard();

	al_set_new_display_flags(ALLEGRO_OPENGL);
	al_set_new_display_option(ALLEGRO_DEPTH_SIZE, 16, ALLEGRO_SUGGEST);
	display = al_create_display(320, 480);
	if(!display) {
		abort_example("Could not create display.\n");
	}

	timer = al_create_timer(1. / 30.);

	queue = al_create_event_queue();
	al_register_event_source(queue,al_get_keyboard_event_source());
	al_register_event_source(queue,al_get_display_event_source(display));
	al_register_event_source(queue,al_get_timer_event_source(timer));

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	/* Really Nice Perspective Calculations */
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	setup_textures(display);
	calculate_frame();
	al_start_timer(timer);

	while(true) {
		al_wait_for_event(queue, &event);
		switch(event.type) 
		{
		case ALLEGRO_EVENT_DISPLAY_CLOSE:
			goto done;

		case ALLEGRO_EVENT_KEY_CHAR:
			if(event.keyboard.keycode == ALLEGRO_KEY_LEFT)
			{
				camera.yangle += angle_speed;
			}
			else if(event.keyboard.keycode == ALLEGRO_KEY_RIGHT)
			{
				camera.yangle -= angle_speed;
			}
			else if(event.keyboard.keycode == ALLEGRO_KEY_UP)
			{
				camera.xangle += angle_speed;
			}
			else if(event.keyboard.keycode == ALLEGRO_KEY_DOWN)
			{
				camera.xangle -= angle_speed;
			}
			else if(event.keyboard.keycode == ALLEGRO_KEY_PGUP)
			{
				camera.dist -= dist_speed;
			}
			else if(event.keyboard.keycode == ALLEGRO_KEY_PGDN)
			{
				camera.dist += dist_speed;
			}
			else if(event.keyboard.keycode == ALLEGRO_KEY_A)
			{
				camera.factor += dist_speed;
				//viewmodel.dist += dist_speed;
			}
			else if(event.keyboard.keycode == ALLEGRO_KEY_Z)
			{
				camera.factor -= dist_speed;
				//viewmodel.dist -= dist_speed;
			}
			else if(event.keyboard.keycode == ALLEGRO_KEY_Q)
			{
				camera.zangle += angle_speed;
			}
			else if(event.keyboard.keycode == ALLEGRO_KEY_W)
			{
				camera.zangle -= angle_speed;
			}
			else if(event.keyboard.keycode == ALLEGRO_KEY_R)
			{
				frame = 0;
				angley_end = -90;
				calculate_frame();
			}
			else if(event.keyboard.keycode == ALLEGRO_KEY_4)
			{
				frame = 0;
				angley_end = 90;
				calculate_frame();
			}

			break;

		case ALLEGRO_EVENT_TIMER:

			if(al_is_event_queue_empty(queue)) {
				if(frame >= 0 && frame <= frame_num*2 )
				{
					render_frame(frame);
				}
				else
				{
					set_camera_position();
				}
				frame++;
				draw();
				al_flip_display();
			}
			break;
		}
	}

done:
	al_destroy_bitmap(bmp);

	return 0;
}

