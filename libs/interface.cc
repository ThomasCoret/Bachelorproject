#include "interface.h"

GLfloat interface::dlr;
GLfloat interface::dlg;
GLfloat interface::dlb;

//ambient light color variables
GLfloat interface::alr;
GLfloat interface::alg;
GLfloat interface::alb;

//light position variables
GLfloat interface::lx;
GLfloat interface::ly;
GLfloat interface::lz;
GLfloat interface::lw;

//interface lighting flags
int interface::aflag;
int interface::dflag;
int interface::mflag;
float interface::g_posX;
float interface::g_posY;
float interface::g_posZ;
float interface::g_orientation; // y axis

interface::interface(world* World_){

	World = World_;

	dlr = 1.0;
	dlg = 1.0;
	dlb = 1.0;

	//ambient light color variables
	alr = 1.0;
	alg = 1.0;
	alb = 1.0;

	//light position variables
	lx = 0.0;
	ly = 0.0;
	lz = 1.0;
	lw = 0.0;

	//interface lighting flags
	aflag = 1;
	dflag = 0;
	mflag = 0;

	g_posX = 0.0;
	g_posY = 25.0;
	g_posZ = 50.0;
	g_orientation = 15.0; // y axis

	/*
	//commonly used material values
	no_mat = { 0.0, 0.0, 0.0, 1.0 };
	default_ambient = {0.2, 0.2, 0.2, 1.0};
	mat_ambient = { 0.7, 0.7, 0.7, 1.0 };
	mat_ambient_color = { 0.8, 0.8, 0.2, 1.0 };
	mat_diffuse = { 0.1, 0.5, 0.8, 1.0 };
	default_diffuse = {0.8, 0.8, 0.8, 1.0};
	mat_specular = { 1.0, 1.0, 1.0, 1.0 };
	no_shininess = { 0.0 };
	low_shininess = { 5.0 };
	high_shininess = { 100.0 };
	mat_emission = {0.3, 0.2, 0.2, 0.0};
	*/
}

void interface::update(){
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRotatef(g_orientation, 0.0, 1.0, 0.0); // rotate in y axis
	glTranslatef(-g_posX, -g_posY, -g_posZ);

	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);
	/*
	GLfloat DiffuseLight[] = {dlr, dlg, dlb}; //set DiffuseLight[] to the specified values
    GLfloat AmbientLight[] = {alr, alg, alb}; //set AmbientLight[] to the specified values
    glLightfv (GL_LIGHT0, GL_DIFFUSE, DiffuseLight); //change the light accordingly
    glLightfv (GL_LIGHT1, GL_AMBIENT, AmbientLight); //change the light accordingly
    GLfloat LightPosition[] = {lx, ly, lz, lw}; //set the LightPosition to the specified values
    glLightfv (GL_LIGHT0, GL_POSITION, LightPosition); //change the light accordingly
	*/
	//setmaterials();

	//draw robots and food
	drawrobots();
	glutSwapBuffers();
}

void interface::init(){
	glEnable (GL_DEPTH_TEST); //enable the depth testing
    glEnable (GL_LIGHTING); //enable the lighting
    glEnable (GL_LIGHT0); //enable LIGHT0, our Diffuse Light
    glEnable (GL_LIGHT1); //enable LIGHT1, our Ambient Light
    glShadeModel (GL_SMOOTH); //set the shader to smooth shader
}

void interface::start(int argc, char *argv[]){
	//generational ? generationallearning(World) : reinforcementlearning(World);

	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_DEPTH); //set the display to Double buffer, with depth
    glutInitWindowSize (500, 500); //set the window size
    glutInitWindowPosition (650, 200); //set the position of the window
	glutCreateWindow("Robot simulation v1.0 made by Thomas Coret");
	init (); //call the init function
	glMatrixMode(GL_PROJECTION);
	gluPerspective(120.0, 1.0, 1.0, 1000.0);
	glEnable(GL_DEPTH_TEST);
	//glutDisplayFunc(&update);
	//glutKeyboardFunc(&keyboard);
	//glutTimerFunc(50, &timer, 0);
	glutMainLoop();
}

void interface::drawrobots(){
	for(auto robot : World->robots){

	}
}