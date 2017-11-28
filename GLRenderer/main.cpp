
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
//void Keyboard(unsigned char key, int x, int y);
//void Mouse(int button, int state, int x, int y);
void InputProcess();

int main(int argc, char *argv[])
{
	renderer.Init(argc, argv, GLUT_RGBA | GLUT_SINGLE, 100, 100, 640, 480, "JiGe's OpenGL Program");

	renderer.SetDisplayFunc(Render);

	renderer.SetIdleFunc(Idle);

	//renderer.SetKeyboardFunc(Keyboard);

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
	cam.SetPosition(100.0f, 0, 0);
	cam.SetLookAt(0, 0, 0);
	cam.SetViewAngle(60.0f,1.3333f);

}

void Render()
{
	renderer.Clear();
	InputProcess();
	renderer.SetTargetMesh(&mesh1);
	renderer.SetCamera(&cam);
	renderer.Render();
}

void Idle()
{
	//static float angle = 0.0f;
	//angle += 0.0005f;
	//if (angle > Math::PI*2.0f)angle = 0.0f;
	//cam.SetPosition(70.0f * cos(angle), 50.0f, 70.0f * sin(angle));
	renderer.Present();
}

void InputProcess()
{
	static int frameCount = 0;
	static POINT prevPos = { 0.0,0.0 };
	POINT pos;
	GetCursorPos(&pos);

	//MOUSE
	if (frameCount != 0)
	{
		int deltaX = pos.x - prevPos.x;
		int deltaY = pos.y - prevPos.y;
		cam.RotateY_Yaw(float(deltaX) / 100.0f);
		cam.RotateX_Pitch(float(deltaY) / 100.0f);
		prevPos = pos;
	}
	else
	{
		//init of prevPos
		prevPos = pos;
	}

	//KEYBOARD
#define IS_KEY_DOWN(key) GetAsyncKeyState(key) & 0x8000
	if (IS_KEY_DOWN('W'))cam.fps_MoveForward(0.005f, false);
	if (IS_KEY_DOWN('A'))cam.fps_MoveRight(-0.005f, false);
	if (IS_KEY_DOWN('S'))cam.fps_MoveForward(-0.005f, false);
	if (IS_KEY_DOWN('D'))cam.fps_MoveRight(0.005f, false);
	if (IS_KEY_DOWN(VK_SPACE))cam.fps_MoveUp(0.005f);
	if (IS_KEY_DOWN(VK_LCONTROL))cam.fps_MoveUp(-0.005f);

	frameCount++;
}
