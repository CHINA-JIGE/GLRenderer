#include "RendererGL.h"

IRenderer renderer;

void Render();
void Init();

int main(int argc, char *argv[])
{
	renderer.Init(argc, argv, GLUT_RGBA | GLUT_SINGLE, 100, 100, 640, 480, "JiGe's OpenGL Program");

	renderer.SetDisplayFunc(Render);

	Init();

	glutMainLoop();

	return 0;

}

void Init()
{
	renderer.LoadGeometry();
}

void Render()
{
	renderer.Clear();
	renderer.Draw();
	renderer.Present();
}