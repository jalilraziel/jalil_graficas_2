#pragma once
#include <math.h>
#include <iostream>
struct Vector3
{
	float X, Y, Z;
};

struct Matriz_4x4
{
	float M00, M01, M02, M03;
	float M10, M11, M12, M13;
	float M20, M21, M22, M23;
	float M30, M31, M32, M33;
}; 

void Imprimir_Matriz(Matriz_4x4);
void Traslacion_Matriz(Matriz_4x4, float, float, float);
void Traslacion_Matriz(Matriz_4x4, Vector3);
void Escalacion_Matriz(Matriz_4x4, float, float, float);
void RotacionXD_Matriz(Matriz_4x4, float);
void RotacionXI_Matriz(Matriz_4x4, float);
void RotacionYD_Matriz(Matriz_4x4, float);
void RotacionYI_Matriz(Matriz_4x4, float);
void RotacionZD_Matriz(Matriz_4x4, float);
void RotacionZI_Matriz(Matriz_4x4, float);
void Trasposicion_Matriz(Matriz_4x4, Matriz_4x4);
void Imprimer_Vector3(Vector3);