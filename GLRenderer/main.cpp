#include "RendererGL.h"

IRenderer renderer;
static float angle=0.0f;

void Render();
void Init();
void Idle();

int main(int argc, char *argv[])
{
	renderer.Init(argc, argv, GLUT_RGBA | GLUT_SINGLE, 100, 100, 512, 512, "JiGe's OpenGL Program");

	renderer.SetDisplayFunc(Render);

	renderer.SetIdleFunc(Idle);

	Init();

	glutMainLoop();

	return 0;

}

void Init()
{
	std::vector<Vertex> geometry;
	int edgeCount = 50;
	for (int i = 0; i < edgeCount; ++i)
	{
		float angle1 = Math::PI* (2.0f / float(edgeCount)) * i;
		float angle2 = Math::PI* (2.0f / float(edgeCount)) * (i+1);

		Vertex v1, v2, v3;
		v1.pos = { 0,0,0 };
		v1.color = { 1.0f,1.0f,1.0f };
		v1.texcoord = {0,0 };
		v2.pos = { cosf(angle1),sinf(angle1),0 };
		v2.color = { cosf(angle1),cos(angle1 + Math::PI / 4.0f),sinf(angle1) };
		v2.texcoord = {1,0 };
		v3.pos = { cosf(angle2),sinf(angle2),0 };
		v3.color = { cosf(angle2),cos(angle2 + Math::PI / 4.0f),sinf(angle2) };
		v3.texcoord = { 0,1 };

		geometry.push_back(v1);
		geometry.push_back(v2);
		geometry.push_back(v3);
	}

	renderer.LoadGeometry(geometry);
}

void Render()
{
	renderer.Clear();
	renderer.Draw(angle);
}

void Idle()
{
	angle += 0.001f;
	renderer.Present();

}