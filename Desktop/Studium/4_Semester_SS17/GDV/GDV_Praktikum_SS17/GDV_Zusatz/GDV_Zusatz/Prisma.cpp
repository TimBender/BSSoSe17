#include <freeglut.h>
#include "Prisma.h"

void Quader(GLfloat fSeitenL)
{
	/*flacher Quader: Windmuehle*/

	//Vorderseite
	glBegin(GL_TRIANGLES);
	glVertex3f(0.2* fSeitenL, 0.2, 0.1);
	glVertex3f(-0.2* fSeitenL, 0.2, 0.1);
	glVertex3f(0, 0, 0);
	glEnd();
	
	//Hinterseite
	glBegin(GL_TRIANGLES);
	glVertex3f(0.2* fSeitenL, 0.2, 0);
	glVertex3f(-0.2* fSeitenL, 0.2, 0);
	glVertex3f(0, 0, 0);
	glEnd();

	//rechte Seite
	glBegin(GL_TRIANGLES);
	glVertex3f(0.2* fSeitenL, 0.2, 0);
	glVertex3f(0.2* fSeitenL, 0.2, 0.1);
	glVertex3f(0, 0, 0);
	glEnd();

	//linke Seite
	glBegin(GL_TRIANGLES);
	glVertex3f(-0.2* fSeitenL, 0.2, 0.1);
	glVertex3f(-0.2* fSeitenL, 0.2, 0);
	glVertex3f(0, 0, 0);
	glEnd();

	//obere Seite/ Deckel
	glBegin(GL_QUADS);
	glVertex3f(0.2* fSeitenL, 0.2, 0);
	glVertex3f(0.2* fSeitenL, 0.2, 0.1);

	glVertex3f(-0.2* fSeitenL, 0.2, 0.1);
	glVertex3f(-0.2* fSeitenL, 0.2, 0);
	
	glEnd();

	return;
}
void Windmuehle(GLfloat fSeitenL){

	glPushMatrix();
	Quader(fSeitenL);
	glPopMatrix();

	glRotatef(45, 0, 0, 1);
	glPushMatrix();
	Quader(fSeitenL);
	glPopMatrix();

	glRotatef(45, 0, 0, 1);
	glPushMatrix();
	Quader(fSeitenL);
	glPopMatrix();

	glRotatef(45, 0, 0, 1);
	glPushMatrix();
	Quader(fSeitenL);
	glPopMatrix();

	glRotatef(45, 0, 0, 1);
	glPushMatrix();
	Quader(fSeitenL);
	glPopMatrix();

	glRotatef(45, 0, 0, 1);
	glPushMatrix();
	Quader(fSeitenL);
	glPopMatrix();

	glRotatef(45, 0, 0, 1);
	glPushMatrix();
	Quader(fSeitenL);
	glPopMatrix();

	glRotatef(45, 0, 0, 1);
	glPushMatrix();
	Quader(fSeitenL);
	glPopMatrix();

	glPushMatrix();
	glutSolidSphere(fSeitenL / 6, 400, 20);
	glPopMatrix();
}