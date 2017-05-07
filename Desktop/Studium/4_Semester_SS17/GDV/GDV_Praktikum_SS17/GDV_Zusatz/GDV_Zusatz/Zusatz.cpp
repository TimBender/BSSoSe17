
#include <freeglut.h>         //lädt alles für OpenGL
#include "BearFigure.h" //Bestandteile des Baeren
#include <iostream> 
#include <thread>         // std::this_thread::sleep_for
#include <chrono> 
#include<cstdlib>
#include<ctime>
#include <cmath>
#include<iostream>
using namespace std;
float nodding_motion_rotate = 1.3, fRotate = 0.f;
void Init()
{
	// Hier finden jene Aktionen statt, die zum Programmstart einmalig 
	// durchgeführt werden müssen

	glClearColor(0.5, 0.05, 0.0, 1.0); // schreibt Farbe in den Buffer

	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	GLfloat diffuseLight[] = { 1., 1.0, 0.9, 1. };
	GLfloat ambientLight[] = { 0.5, 0.4, 0.4, 0.1 };

	GLfloat lightPos[] = { 0, 14, 1.2, 1 }; //x,y,z
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);

	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
}

void RenderScene() //Zeichenfunktion
{
	const float CUBIC_SIZE = 0.4f; //Kugelgroesse (Koerper, Kopf etc)

	// Hier befindet sich der Code der in jedem Frame ausgefuehrt werden muss
	glLoadIdentity();   // Aktuelle Model-/View-Transformations-Matrix zuruecksetzen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	gluPerspective(20., 1., 0.1, 10.);

	//Kameraeinstellung
	gluLookAt(0., 0., 8., 0., 0., 0., 0., 1., 0);

	//Farben fuer den Baeren
	GLfloat custom_beige[] = { 1.f, 1.f, 1.f, 1.0f };
	GLfloat custom_lbeige[] = { 0.8f, 0.7f, 0.5f, 1.0f };

	GLfloat custom_green[] = { 0.f, 0.7f, 0.8f, 1.0f };
	GLfloat custom_black[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat custom_white[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	//Toenung fuer alle Objekte
	GLfloat custom_red_dawn[] = { 0.12f, 0.0f, 0.0f, 1.f };

	glMaterialfv(GL_FRONT, GL_AMBIENT, custom_red_dawn); // Abendlicht simulieren

	glNormal3f(0, 1, 1);

	glMaterialfv(GL_FRONT, GL_DIFFUSE, custom_lbeige);


	glMaterialfv(GL_FRONT, GL_DIFFUSE, custom_lbeige);
	//Koerper
	glPushMatrix();
	glColor3f(0.8f, 0.6f, 0.4f);
	glTranslatef(0, -CUBIC_SIZE*1.6, 0);
	glutSolidSphere(CUBIC_SIZE + CUBIC_SIZE / 4, 400, 20);
	glPopMatrix();

	//Fliege
	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_DIFFUSE, custom_black);
	glTranslatef(0., -CUBIC_SIZE, 0);
	BearFigure::Fliege(0.6);
	glPopMatrix();


	// Wackeldackel Bewegung fuer den Baerenkopf
	glRotatef(-nodding_motion_rotate, 1.0, 0.0, 0);
	BearFigure::BearHead(fRotate, CUBIC_SIZE);

	glutSwapBuffers();
}

void Reshape(int width, int height)
{
	// Hier finden die Reaktionen auf eine Veränderung der Größe des 
	// Graphikfensters statt
}
int roundR = 1;

void Animate(int value)
{
	// Hier werden Berechnungen durchgeführt, die zu einer Animation der Szene  
	// erforderlich sind. Dieser Prozess läuft im Hintergrund und wird alle 
	// 1000 msec aufgerufen. Der Parameter "value" wird einfach nur um eins 
	// inkrementiert und dem Callback wieder uebergeben. 
	//std::cout << "value=" << value << std::endl;
	// RenderScene aufrufen
	glutPostRedisplay();

	//Rotatiere Windmuehle
	fRotate += 5.0f;

	//Kopfbewegung des Bären: Wackeldackel Simulation
	nodding_motion_rotate = (nodding_motion_rotate + 0.25); // Rotationswinkel aendern
	if (nodding_motion_rotate > 15 || nodding_motion_rotate < -2.f) {
		for (float i = 0; i < nodding_motion_rotate + 2.f;){
			//std::this_thread::sleep_for(std::chrono::milliseconds(2)); // kurzes Stoppen: "Einrasteffekt"
			nodding_motion_rotate -= i;
			i += 0.01;
		}

	}
	// Timer wieder registrieren - Animate wird so nach 10 msec mit value+=1 aufgerufen.
	int wait_msec = 10;
	glutTimerFunc(wait_msec, Animate, ++value);
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);                // GLUT initialisieren
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 800);         // Fenster-Konfiguration
	glutCreateWindow("Tran; Goldschmidt");   // Fenster-Erzeugung
	glutDisplayFunc(RenderScene);         // Zeichenfunktion bekannt machen
	glutReshapeFunc(Reshape);
	// TimerCallback registrieren; wird nach 10 msec aufgerufen mit Parameter 0  
	glutTimerFunc(10, Animate, 0);
	Init();
	glutMainLoop();
	return 0;
}
