#include "libs/worldmanager.h"
#include "GL/glut.h"
#define _USE_MATH_DEFINES

using namespace std;

#define generations 200
#define trainingiterations 500

world World;

bool randomized = false;

int it = 0;
int gen = 0;

//diffuse light color variables
GLfloat dlr = 1.0;
GLfloat dlg = 1.0;
GLfloat dlb = 1.0;

//ambient light color variables
GLfloat alr = 1.0;
GLfloat alg = 1.0;
GLfloat alb = 1.0;

//light position variables
GLfloat lx = 0.0;
GLfloat ly = 0.0;
GLfloat lz = 1.0;
GLfloat lw = 0.0;

//interface lighting flags
int aflag = 0;
int dflag = 1;
int mflag = 0;

//commonly used material values
GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
GLfloat default_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
GLfloat mat_ambient[] = { 0.7, 0.7, 0.7, 1.0 };
GLfloat mat_ambient_color[] = { 0.8, 0.8, 0.2, 1.0 };
GLfloat mat_diffuse[] = { 0.1, 0.5, 0.8, 1.0 };
GLfloat default_diffuse[] = { 0.8, 0.8, 0.8, 1.0 };
GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat no_shininess[] = { 0.0 };
GLfloat low_shininess[] = { 5.0 };
GLfloat high_shininess[] = { 100.0 };
GLfloat mat_emission[] = { 0.3, 0.2, 0.2, 0.0 };

float g_posX = -80.0, g_posY = 80.0, g_posZ = World.width/2;
float g_orientation = 90.0; // y axis

void generationallearning2(bool, string);
void generationallearning(bool, string);
void init();
void keyboard(unsigned char, int, int);
void update();
void drawrobots();
void timer(int);
int menu(int, char**);


int main(int argc, char *argv[]){
	srand (time(NULL));

	menu(argc, argv);
	
	return 1;
}

void newgen(){
	World.updaterobots((float)1/generations);
	World.randomizeworld(0);
}

void rungen(){
	int iterations = 0;
	cout<<"how many iterations?\n";
	cin>>iterations;
	if(iterations<0){
		cout<<"iterations < 0 not possible\n";
		return;
	}
	while (!World.done()&& iterations > 0){
		World.simulate();
		iterations--;
	}
}

void drawinglut(int argc, char *argv[]){
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_DEPTH); //set the display to Double buffer, with depth
    glutInitWindowSize (500, 500); //set the window size
    glutInitWindowPosition (650, 200); //set the position of the window
	glutCreateWindow("Robot simulation v1.0 made by Thomas Coret");
	init (); //call the init function
	glMatrixMode(GL_PROJECTION);
	gluPerspective(120.0, 1.0, 1.0, 1000.0);
	glEnable(GL_DEPTH_TEST);
	glutDisplayFunc(&update);
	glutKeyboardFunc(&keyboard);
	glutTimerFunc(50, &timer, 0);
	glutMainLoop();
}

void save(){
	string filename;
	cout<<"How do u want to name the file?\n";
	cin>>filename;
	string finalname = "robotsaves/" + filename;
	if(!(filename == "")){
		World.savebestrobot(finalname);
		cout<<"saved in:"<<finalname<<endl;
	}
	else
		cout<<"string is empty\n";
}

void load(){
	string filename;
	cout<<"What is the name of the file (in robotsaves)?\n";
	cin>>filename;
	string finalname = "robotsaves/" + filename;
	if(!(filename == "")){
		World.loadrobot(finalname);
		cout<<"loaded from: "<<finalname<<endl;
	}
	else
		cout<<"string is empty\n";
}

void experiment(){
	bool savefile = false;
	char yesorno;
	string filename = "";
	string finalname = "graphs/socialexperiment/";
	cout<<"save performance? (y/n)\n";
	cin>>yesorno;
	yesorno = toupper(yesorno);
	if(yesorno == 'Y'){
		savefile = true;
		cout<<"How do u want to name the file?\n";
		cin>>filename;
		finalname += filename;
		cout<<"saving in :"<<finalname<<endl;
	}
	generationallearning2(savefile, finalname);
}

int menu(int argc, char *argv[]){
	bool done = false;
	while(!done){
		cout<<"Welcome to the menu. Choose by typing the letter between the brackets\n"
			<<"(R)un the current standard experiment, simulate the (c)urrent generation, Render current generation in (g)lut, (M)ake a new generation of the current generations best robot, (S)ave the current generations best robot, (L)oad in a saved robot, (Q)uit\n";
		char input;
		std::cin>>input;
		input = toupper(input);
		switch(input){
			//run the current gen
			case 'R':
				std::cout<<"experiment\n";
				experiment();
				break;
			case 'C':
				std::cout<<"simgen\n";
				rungen();
				break;
			//glut
			case 'G':
				std::cout<<"glut\n Press r to reset and randomize the food.\n";
				drawinglut(argc, argv);
				break;
			//new generation
			case 'M':
				std::cout<<"newgen\n";
				newgen();
				break;
			//save best robot
			case 'S':
				std::cout<<"save\n";
				save();
				break;
			//load robot
			case 'L':
				std::cout<<"load\n";
				load();
				break;
			case 'Q':
				std::cout<<"quit\n";
				done = true;
				break;
			default:
				cout<<"wrong input"<<std::endl;
				break;

		}
	}
}

//train robots generationally in the same world
void generationallearning(bool save, string filename){
	int its = trainingiterations;
	//always need to declarate outputfile
	ofstream outputfile;
	//output to file
	if(save)
		outputfile.open(filename);
	//test the trained robots
	for(int i = 0; i < generations; i++){
		while (!World.done()&& its > 0){
			//World.drawworld();
			World.simulate();
			its--;
		}
		cout<<"generation: "<<i<<" food left: "<<World.nfood<<endl;
		//update the robots at the end of the generation
		World.updaterobots((float)1/generations);
		//reset total iterations
		its = trainingiterations;
		//save to file
		if(save)
			outputfile<<i<<";"<<World.getaveragefitness()<<";"<<World.getmaxfitness()<<"\n";
		//randomize
		World.randomizeworld(0);
	}
	if(save)	
		outputfile.close();
	
}

//train robots seperately in their own world
void generationallearning2(bool save, string filename){
	//amount of parallell worlds training robots
	bool identicalrobots = true;
	int nworlds = 20;
	worldmanager WM(nworlds, generations, identicalrobots);

	//always need to declarate outputfile
	ofstream outputfile;
	//output to file
	if(save)
		outputfile.open(filename);

	for(int i = 0; i <10; i++){
		outputfile<<i<<":";
		for(int k = 0; k < generations; k++){
			WM.resetfitness();

			for(int j = 0; j < trainingiterations; j++){
				WM.simulate();
			}

			WM.update();

			if(save)
				outputfile<<WM.maxfitness<<";";

			cout<<"generation "<<k<<" done. max fitness: "<<WM.maxfitness<<", avg fitness: "<<WM.averagefitness/nworlds<<", learningrate: "<<WM.Worlds[0].robots[0].learningrate<<std::endl;
		}
		outputfile<<"\n";
		float inputith[MAX][MAX];
		float inputhto[MAX][MAX];
		//copy the best robot to our single world to view it in glut or save it
		WM.gethto(inputhto);
		WM.getith(inputith);
		World.newhto(inputhto);
		World.newith(inputith);
		World.clonerobots();
		string robotname = "robotsaves/setting5/";
		robotname += to_string(i);
		robotname += ".bot";
		World.savebestrobot(robotname);
		//randomize robots
		WM.randomizerobots();
	}
	if(save)	
		outputfile.close();

	float inputith[MAX][MAX];
	float inputhto[MAX][MAX];
	//copy the best robot to our single world to view it in glut or save it
	WM.gethto(inputhto);
	WM.getith(inputith);
	World.newhto(inputhto);
	World.newith(inputith);
	World.clonerobots();
}

/************************************************  Glut stuff  ************************************************/

void timer(int value){
	if(World.done())
		randomized = false;
	if(!randomized){
		World.randomizeworld(0);
		randomized = true;
	}
	World.simulate();
	//string faka;
	//cin>>faka;
	it++;
	glutPostRedisplay();
	glutTimerFunc(20, &timer, 0);
}

void drawOneParticle()
{
	glutSolidSphere(2.0, 10, 10);
}

void drawrobotsandfood(){
	float nrays = 30;
	for(auto robot : World.robots){
		//draw robot
		glPushMatrix();
		glTranslatef(0.0, robot.y, robot.x);
		glScalef(1.0,1.0, 1.0);
		glColor3f(0.6, 0.0,0.1);
		drawOneParticle();
		glPopMatrix();	
		//draw field of view
		glPushMatrix();
		glLineWidth(0.5);
		float degree = robot.rotation -90;
		glBegin(GL_LINES);
		for(int i = 0; i<nrays; i++){
			if(i<=nrays/3)
				glColor3f(0.6, 0.5,0.1);
			if(i>nrays/3 && i<= nrays*2/3) 
				glColor3f(0.6, 0.0,0.1);
			if(i>nrays*2/3)
				glColor3f(0.6, 0.0,0.6);
			glVertex3f(0.0, robot.y, robot.x);
			glVertex3f(0.0, robot.y +( 20 * sin(degree*M_PI/180)), robot.x + (20 * cos(degree*M_PI/180)));
			degree += 180/nrays;
		}
		glEnd();
		glPopMatrix();	
	}
	for(auto food : World.foods){
		glPushMatrix();
		glTranslatef(0.0, food.y, food.x);
		glScalef(1.0,1.0, 1.0);
		glColor3f(0.5, 0.8, 0.3);
		drawOneParticle();
		glPopMatrix();	
	}
}

void drawground(){
	glColor3f(1.0, 1.0, 1.0);
   // ground plane
   glBegin(GL_LINE_STRIP);
   glVertex3f(0.0, 0.0, 0.0  );
   glVertex3f(0.0, 0.0, World.width);
   glVertex3f(0.0, World.height, World.width);
   glVertex3f(0.0, World.height, 0 );
   glVertex3f(0.0, 0.0, 0.0  );
   glEnd();
}

void setmaterials( ){
	glPushMatrix();
	glTranslatef(lx,ly,lz);
	if(mflag==1)
	{
	   glMaterialfv(GL_FRONT, GL_AMBIENT, default_ambient);
       glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
       glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
       glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
       glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
	}
	else
	{
       glMaterialfv(GL_FRONT, GL_AMBIENT, default_ambient);
       glMaterialfv(GL_FRONT, GL_DIFFUSE, default_diffuse);
       glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
       glMaterialfv(GL_FRONT, GL_SHININESS, no_shininess);
       glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
	}

	glPopMatrix();
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
		case 'r': // randomize the world
			World.randomizeworld(0);
		    break;
		case 'w': // up
			   g_posY = g_posY + 1.0;
			   break;
		case 's': // down
			   g_posY = g_posY - 1.0;
			   break;
		case 'a': // left
			   g_orientation = g_orientation - 15.0;
			   break;
		case 'd': // right
			   g_orientation = g_orientation + 15.0;
			   break;
		case 'f': // forwards
			   g_posX = g_posX + sin(g_orientation / 180.0 * M_PI);
			   g_posZ = g_posZ - cos(g_orientation / 180.0 * M_PI);
			   break;
		case 'v': //backwards
			   g_posX = g_posX - sin(g_orientation / 180.0 * M_PI);
			   g_posZ = g_posZ + cos(g_orientation / 180.0 * M_PI);
			   break;
	}
}

void update(){
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRotatef(g_orientation, 0.0, 1.0, 0.0); // rotate in y axis
	glTranslatef(-g_posX, -g_posY, -g_posZ);

	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);
	
	GLfloat DiffuseLight[] = {dlr, dlg, dlb}; //set DiffuseLight[] to the specified values
    GLfloat AmbientLight[] = {alr, alg, alb}; //set AmbientLight[] to the specified values
    glLightfv (GL_LIGHT0, GL_DIFFUSE, DiffuseLight); //change the light accordingly
    glLightfv (GL_LIGHT1, GL_AMBIENT, AmbientLight); //change the light accordingly
    GLfloat LightPosition[] = {lx, ly, lz, lw}; //set the LightPosition to the specified values
    glLightfv (GL_LIGHT0, GL_POSITION, LightPosition); //change the light accordingly
	
	//setmaterials();
	//draw ground, robots and food
	drawground();
	drawrobotsandfood();
	glutSwapBuffers();
}

void init(){
	glEnable (GL_DEPTH_TEST); //enable the depth testing
    glEnable (GL_LIGHTING); //enable the lighting
    glEnable (GL_LIGHT0); //enable LIGHT0, our Diffuse Light
    glEnable (GL_LIGHT1); //enable LIGHT1, our Ambient Light
    glShadeModel (GL_SMOOTH); //set the shader to smooth shader
    glEnable(GL_COLOR_MATERIAL);
}

