
#include "pch.h"
#include "RendererGL.h"

IRenderer renderer;
IMesh mesh1;
ICamera cam;
ITexture diffuseMap;
static float angle=0.0f;

void Render();
void Init();
void Idle();

int main(int argc, char *argv[])
{
	renderer.Init(argc, argv, GLUT_RGBA | GLUT_SINGLE, 100, 100, 640, 480, "JiGe's OpenGL Program");

	renderer.SetDisplayFunc(Render);

	renderer.SetIdleFunc(Idle);

	Init();

	glutMainLoop();

	return 0;

}

void Init()
{
	//diffuseMap.LoadPPM("../media/earth.ppm");
	diffuseMap.LoadBMP("../media/rock.bmp");
	//diffuseMap.LoadBMP("../media/grass.bmp");

	//mesh1.CreateSphere(5.0f, 20, 20, false);
	//mesh1.LoadFile_OBJ("../media/star.obj");
	mesh1.LoadFile_OBJ("../media/earth.obj");
	//mesh1.CreateBox(10.0f, 10.0f, 10.0f, 3, 3, 3);
	mesh1.SetTexture(&diffuseMap);
	cam.SetPosition(1.0f, 0, 0);
	cam.SetLookAt(0, 0, 0);
	cam.SetViewAngle(60.0f,1.3333f);

}

void Render()
{
	renderer.Clear();
	renderer.SetTargetMesh(&mesh1);
	renderer.SetCamera(&cam);
	renderer.Render();
}

void Idle()
{
	static float angle = 0.0f;
	angle += 0.0005f;
	if (angle > Math::PI*2.0f)angle = 0.0f;
	cam.SetPosition(70.0f * cos(angle), 50.0f, 70.0f * sin(angle));
	renderer.Present();
}