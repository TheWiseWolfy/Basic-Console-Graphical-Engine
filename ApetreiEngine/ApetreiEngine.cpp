#include <iostream>
#include "olcConsoleGameEngine.h"

using namespace std;

struct vec3d {
	float x, y, z;
};

struct triangle {
	vec3d point[3];
};

struct mesh {
	vector<triangle> tris;
};

struct mat4x4
{
	float mat[4][4] = { 0 };
	//randuri - colonane
};

class olcEngine3D : public olcConsoleGameEngine {
public: 
    olcEngine3D()
    {
        m_sAppName = L"3D Demo";
    }

private:
    mesh meshCube;
	mat4x4 matProj;

	float fTheta;

	void MultiplyMatrixVector(vec3d& i, vec3d& o, mat4x4& mat)
	{
		o.x = i.x * mat.mat[0][0] + i.y * mat.mat[1][0] + i.z * mat.mat[2][0] + mat.mat[3][0];
		o.y = i.x * mat.mat[0][1] + i.y * mat.mat[1][1] + i.z * mat.mat[2][1] + mat.mat[3][1];
		o.z = i.x * mat.mat[0][2] + i.y * mat.mat[1][2] + i.z * mat.mat[2][2] + mat.mat[3][2];
		float w = i.x * mat.mat[0][3] + i.y * mat.mat[1][3] + i.z * mat.mat[2][3] + mat.mat[3][3];

		if (w != 0.0f)
		{
			o.x /= w; o.y /= w; o.z /= w;
		}
	}



    virtual bool OnUserCreate(){
			meshCube.tris = {

			// SOUTH
			{ 0.0f, 0.0f, 0.0f,    0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 0.0f },
			{ 0.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,    1.0f, 0.0f, 0.0f },

			// EAST                                                      
			{ 1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f },
			{ 1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 0.0f, 1.0f },

			// NORTH                                                     
			{ 1.0f, 0.0f, 1.0f,    1.0f, 1.0f, 1.0f,    0.0f, 1.0f, 1.0f },
			{ 1.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,    0.0f, 0.0f, 1.0f },

			// WEST                                                      
			{ 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,    0.0f, 1.0f, 0.0f },
			{ 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 0.0f,    0.0f, 0.0f, 0.0f },

			// TOP                                                       
			{ 0.0f, 1.0f, 0.0f,    0.0f, 1.0f, 1.0f,    1.0f, 1.0f, 1.0f },
			{ 0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 1.0f, 0.0f },

			// BOTTOM                                                    
			{ 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f },
			{ 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f,    1.0f, 0.0f, 0.0f },

			};

			//Projection matrix (multa mate)

			float fNear = 0.1f;
			float fFar = 1000.0f;
			float fFov = 90.0f;
			float fAspectRatio = (float)ScreenHeight() / (float)ScreenWidth();
			//Tangenta importanta - refera documentatia
			float fFovRad = 1.0f / tanf(fFov * 0.5f / 180.0f * 3.14159f);

			//Definirea finala a matrici
			matProj.mat[0][0] = fAspectRatio * fFovRad;
			matProj.mat[1][1] = fFovRad;
			matProj.mat[2][2] = fFar / (fFar - fNear);
			matProj.mat[3][2] = (-fFar * fNear) / (fFar - fNear);
			matProj.mat[2][3] = 1.0f;
			matProj.mat[3][3] = 0.0f;


        return true;
    }
    bool OnUserUpdate(float fElapsedTime){

		Fill(0, 0, ScreenWidth(), ScreenWidth(), PIXEL_SOLID, FG_BLACK);


		// Set up rotation matrices
		mat4x4 matRotZ, matRotX;
		fTheta += 1.0f * fElapsedTime;

		// Rotation Z
		matRotZ.mat[0][0] = cosf(fTheta);
		matRotZ.mat[0][1] = sinf(fTheta);
		matRotZ.mat[1][0] = -sinf(fTheta);
		matRotZ.mat[1][1] = cosf(fTheta);
		matRotZ.mat[2][2] = 1;
		matRotZ.mat[3][3] = 1;

			




		// Rotation X
		matRotX.mat[0][0] = 1;
		matRotX.mat[1][1] = cosf(fTheta * 0.5f);
		matRotX.mat[1][2] = sinf(fTheta * 0.5f);
		matRotX.mat[2][1] = -sinf(fTheta * 0.5f);
		matRotX.mat[2][2] = cosf(fTheta * 0.5f);
		matRotX.mat[3][3] = 1;



		//Aici desenam triunchiuri prin projection

		for (auto tri : meshCube.tris) {
			triangle triProjected, triTranslated, triRotatedZ, triRotatedZX;

			// Rotate in Z-Axis
			MultiplyMatrixVector(tri.point[0], triRotatedZ.point[0], matRotZ);
			MultiplyMatrixVector(tri.point[1], triRotatedZ.point[1], matRotZ);
			MultiplyMatrixVector(tri.point[2], triRotatedZ.point[2], matRotZ);

			// Rotate in X-Axis
			MultiplyMatrixVector(triRotatedZ.point[0], triRotatedZX.point[0], matRotX);
			MultiplyMatrixVector(triRotatedZ.point[1], triRotatedZX.point[1], matRotX);
			MultiplyMatrixVector(triRotatedZ.point[2], triRotatedZX.point[2], matRotX);

			// Offset into the screen
			triTranslated = triRotatedZX;
			triTranslated.point[0].z = triRotatedZX.point[0].z + 3.0f;
			triTranslated.point[1].z = triRotatedZX.point[1].z + 3.0f;
			triTranslated.point[2].z = triRotatedZX.point[2].z + 3.0f;



			// Project triangles from 3D --> 2D
			MultiplyMatrixVector(triTranslated.point[0], triProjected.point[0], matProj);
			MultiplyMatrixVector(triTranslated.point[1], triProjected.point[1], matProj);
			MultiplyMatrixVector(triTranslated.point[2], triProjected.point[2], matProj);

			triProjected.point[0].x += 1.0f; triProjected.point[0].y += 1.0f;
			triProjected.point[1].x += 1.0f; triProjected.point[1].y += 1.0f;
			triProjected.point[2].x += 1.0f; triProjected.point[2].y += 1.0f;


			triProjected.point[0].x *= 0.5f * (float)ScreenWidth();
			triProjected.point[0].y *= 0.5f * (float)ScreenHeight();
			triProjected.point[1].x *= 0.5f * (float)ScreenWidth();
			triProjected.point[1].y *= 0.5f * (float)ScreenHeight();
			triProjected.point[2].x *= 0.5f * (float)ScreenWidth();
			triProjected.point[2].y *= 0.5f * (float)ScreenHeight();

			DrawTriangle(triProjected.point[0].x, triProjected.point[0].y,
				triProjected.point[1].x, triProjected.point[1].y,
				triProjected.point[2].x, triProjected.point[2].y,
				PIXEL_SOLID, FG_WHITE);
				

		}

		return true;

    }


};

int main()
{
    olcEngine3D demo;
    if (demo.ConstructConsole(256, 240, 4, 4)) {
        demo.Start();
    }

}

