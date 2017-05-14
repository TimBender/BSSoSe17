#pragma once

class BearFigure{
public:
	static void BearHead(const float& rotate, GLfloat fSeitenL);
	static void Fliege(GLfloat fSeitenL);
private:
	static void draw(GLfloat fSeitenL);
};

