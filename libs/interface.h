#ifndef INTERFACE_H
#define INTERFACE_H

#include "world.h"
#include "GL/glut.h"
#define _USE_MATH_DEFINES

class interface{
	//variables
	public:
		//diffuse light color variables
		static GLfloat dlr;
		static GLfloat dlg;
		static GLfloat dlb;

		//ambient light color variables
		static GLfloat alr;
		static GLfloat alg;
		static GLfloat alb;

		//light position variables
		static GLfloat lx;
		static GLfloat ly;
		static GLfloat lz;
		static GLfloat lw;

		//interface lighting flags
		static int aflag;
		static int dflag;
		static int mflag;
		static float g_posX, g_posY, g_posZ;
		static float g_orientation; // y axis
		/*
		//commonly used material values
		GLfloat no_mat[4];
		GLfloat default_ambient[4];
		GLfloat mat_ambient[4];
		GLfloat mat_ambient_color[4];
		GLfloat mat_diffuse[4];
		GLfloat default_diffuse[4];
		GLfloat mat_specular[4];
		GLfloat no_shininess[1];
		GLfloat low_shininess[1];
		GLfloat high_shininess[1];
		GLfloat mat_emission[4];
		*/
	private:
		world *World;

		
	
	//functionS
	public:
		interface(world*);
		void start(int, char**);
		void update();
	private:
		void drawrobots();
		void init();
};



#endif

