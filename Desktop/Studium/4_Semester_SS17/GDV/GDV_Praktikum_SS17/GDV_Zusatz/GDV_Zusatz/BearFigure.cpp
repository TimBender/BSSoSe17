#include <freeglut.h>
#include "BearFigure.h"
#include "Prisma.h"

/** Kopf des Baeren: Groesse skalierbar mit: fSeitenL */
void BearFigure::BearHead(const float& rotate, GLfloat fSeitenL)
{
	const float STICK_SIZE = 0.1f; //Laenge fuer den Muetzenstiel fuer den Propeller

	//Farben fuer den Baeren
	GLfloat custom_beige[] = { 1.f, 1.f, 1.f, 1.0f };
	GLfloat custom_lbeige[] = { 0.8f, 0.7f, 0.5f, 1.0f };
	GLfloat custom_green[] = { 0.f, 0.7f, 0.8f, 1.0f };
	GLfloat custom_black[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat custom_white[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	/*Ohr links, rechts*/
	glMaterialfv(GL_FRONT, GL_DIFFUSE, custom_lbeige);
	glPushMatrix();
	glTranslatef(-fSeitenL / 1.5, fSeitenL, -fSeitenL / 10);
	glutSolidSphere(fSeitenL / 2.5, 400, 20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(fSeitenL / 1.5, fSeitenL, -fSeitenL / 10);
	glutSolidSphere(fSeitenL / 2.5, 400, 20);
	glPopMatrix();

	/*Muetze*/
	glPushMatrix();
	glTranslatef(0, fSeitenL - fSeitenL / 8, -fSeitenL / 10);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, custom_black);
	glutSolidSphere(fSeitenL / 2, 600, 20);
	glTranslatef(0, fSeitenL / 2, 0);

	/*Stiel*/
	glRotatef(-90, 1., 0, 0);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, custom_white);
	glutSolidCylinder(0.01, STICK_SIZE, 10, 10);
	glPopMatrix();

	/*Schaedel*/
	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_DIFFUSE, custom_lbeige);
	glutSolidSphere(fSeitenL, 400, 20);
	glPopMatrix();

	glTranslatef(0, -0.05, 0);

	/*Auge links*/
	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_DIFFUSE, custom_green);
	glTranslatef(-fSeitenL / 3, fSeitenL / 5, fSeitenL - fSeitenL / 3);
	glutSolidSphere(fSeitenL / 3.2, 400, 20);

	/*Pupille*/
	glMaterialfv(GL_FRONT, GL_DIFFUSE, custom_black);
	glTranslatef(fSeitenL / 20, 0, 0);
	glutSolidSphere(fSeitenL / 8, 400, 20);
	glPopMatrix();

	/*Auge rechts*/
	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_DIFFUSE, custom_green);
	glTranslatef(fSeitenL / 3, fSeitenL / 5, fSeitenL - fSeitenL / 3);
	glutSolidSphere(fSeitenL / 3.2, 400, 20);

	/*Pupille*/
	glMaterialfv(GL_FRONT, GL_DIFFUSE, custom_black);
	glTranslatef(-fSeitenL / 20, 0, 0);
	glutSolidSphere(fSeitenL / 8, 400, 20);
	glPopMatrix();


	/*Augenbrauen*/
	glPushMatrix();
	glTranslatef(fSeitenL / 10, 0, 0);
	glBegin(GL_POLYGON);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, custom_lbeige);
	glVertex3f(0, fSeitenL / 1.8, fSeitenL);
	glVertex3f(0.2, fSeitenL / 1.9, fSeitenL);
	glVertex3f(0, fSeitenL / 2.4, fSeitenL);
	glVertex3f(0.2, fSeitenL / 2.5, fSeitenL);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-fSeitenL / 10, 0, 0);
	glBegin(GL_POLYGON);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, custom_lbeige);
	glVertex3f(0, fSeitenL / 1.8, fSeitenL);
	glVertex3f(-0.2, fSeitenL / 1.9, fSeitenL);
	glVertex3f(0, fSeitenL / 2.4, fSeitenL);
	glVertex3f(-0.2, fSeitenL / 2.5, fSeitenL);
	glEnd();
	glPopMatrix();

	/*weisse Schnauze*/
	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_DIFFUSE, custom_white);
	glTranslatef(0, -fSeitenL / 4, fSeitenL - fSeitenL / 3);
	glutSolidSphere(fSeitenL / 2, 400, 20);
	glPopMatrix();


	/*Schnauze*/
	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_DIFFUSE, custom_black);
	glTranslatef(0, 0, fSeitenL);
	glutSolidSphere(fSeitenL / 5, 400, 20);
	glPopMatrix();

	/*Windmuehle*/
	glPushMatrix();

	/*Rotiere Windrad im Uhrzeigersinn*/
	glTranslatef(0, (fSeitenL + STICK_SIZE + (fSeitenL / 2)), -fSeitenL / 10); // x= Kopf Radius + Stiel Radius + Muetzen Radius
	glRotatef(-rotate, 0.0, 0.0, 1.0);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, custom_white);
	Windmuehle(0.1);
	glPopMatrix();
}


/**zeichne die FLiege des Baeren, fSeitenL verändert die Breite*/

void BearFigure::Fliege(GLfloat fSeitenL){
	//rechte Seite der Fliege
	glPushMatrix();
	glRotatef(-92, 0., 0., 1.0); //kippe nach rechts
	draw(fSeitenL);
	glPopMatrix();

	//linke Seite der Fliege
	glPushMatrix();
	glRotatef(92, 0., 0., 1.0); //kippe nach links
	draw(fSeitenL);
	glPopMatrix();

	//Knoten in der Mitte
	glPushMatrix();
	glutSolidSphere(fSeitenL /8, 400, 20);
	glPopMatrix();
}

//zeichnet Prisma fuer die Propellerteile
void BearFigure::draw(float fSeitenL){
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
}