#include "matrix.h"
#include <iostream>

void Imprimir_Matriz(Matriz_4x4 A)
{
	std::cout << " " << A.M00 << "  " << A.M01 << "  " << A.M02 << "  " << A.M03 << std::endl;
	std::cout << " " << A.M10 << "  " << A.M11 << "  " << A.M12 << "  " << A.M13 << std::endl;
	std::cout << " " << A.M20 << "  " << A.M21 << "  " << A.M22 << "  " << A.M23 << std::endl;
	std::cout << " " << A.M30 << "  " << A.M31 << "  " << A.M32 << "  " << A.M33 << std::endl;
}

void Traslacion_Matriz(Matriz_4x4 A, float X, float Y, float Z)
{
	A.M03 += X;
	A.M13 += Y;
	A.M23 += Z;
}
void Traslacion_Matriz(Matriz_4x4 A, Vector3 B)
{
	A.M00 += B.X, A.M01 += B.X, A.M02 += B.X, A.M03 += B.X;
	A.M10 += B.Y, A.M11 += B.Y, A.M12 += B.Y, A.M13 += B.Y;
	A.M20 += B.Z, A.M21 += B.Z, A.M22 += B.Z, A.M23 += B.Z;
}

void Escalacion_Matriz(Matriz_4x4 A, float X, float Y, float Z)
{
	A.M00 *= X, A.M01 *= X, A.M02 *= X, A.M03 *= X;
	A.M10 *= Y, A.M11 *= Y, A.M12 *= Y, A.M13 *= Y;
	A.M20 *= Z, A.M21 *= Z, A.M22 *= Z, A.M23 *= Z;
}
void RotacionXD_Matriz(Matriz_4x4 matriz, float ang)
{
	Matriz_4x4 Temp = matriz;
	Temp.M10 = (matriz.M10*cos(ang)) + (matriz.M20*sin(ang)); Temp.M11 = (matriz.M11*cos(ang)) + (matriz.M21*sin(ang));
	Temp.M12 = (matriz.M12*cos(ang)) + (matriz.M22*sin(ang)); Temp.M13 = (matriz.M13*cos(ang)) + (matriz.M23*sin(ang));
	Temp.M20 = (-matriz.M10*sin(ang)) + (matriz.M20*cos(ang)); Temp.M21 = (-matriz.M11*sin(ang)) + (matriz.M21*cos(ang));
	Temp.M22 = (-matriz.M12*sin(ang)) + (matriz.M22*cos(ang)); Temp.M23 = (-matriz.M13*sin(ang)) + (matriz.M23*cos(ang));

	matriz = Temp;
}
void RotacionXI_Matriz(Matriz_4x4 matriz, float ang)
{
	Matriz_4x4 Temp = matriz;
	Temp.M10 = (matriz.M10*cos(ang)) - (matriz.M20*sin(ang)); Temp.M11 = (matriz.M11*cos(ang)) - (matriz.M21*sin(ang));
	Temp.M12 = (matriz.M12*cos(ang)) - (matriz.M22*sin(ang)); Temp.M13 = (matriz.M13*cos(ang)) - (matriz.M23*sin(ang));
	Temp.M20 = (-matriz.M10*sin(ang)) + (matriz.M20*cos(ang)); Temp.M21 = (-matriz.M11*sin(ang)) + (matriz.M21*cos(ang));
	Temp.M22 = (-matriz.M12*sin(ang)) + (matriz.M22*cos(ang)); Temp.M23 = (-matriz.M13*sin(ang)) + (matriz.M23*cos(ang));

	matriz = Temp;
}
void RotacionYD_Matriz(Matriz_4x4 matriz, float ang)
{
	Matriz_4x4 Temp = matriz;
	Temp.M00 = (matriz.M00*cos(ang)) + (matriz.M20*sin(ang)); Temp.M01 = (matriz.M01*cos(ang)) + (matriz.M21*sin(ang));
	Temp.M02 = (matriz.M02*cos(ang)) + (matriz.M22*sin(ang)); Temp.M03 = (matriz.M03*cos(ang)) + (matriz.M23*sin(ang));
	Temp.M10 = (-matriz.M00*sin(ang)) + (matriz.M20*cos(ang)); Temp.M21 = (-matriz.M01*sin(ang)) + (matriz.M21*cos(ang));
	Temp.M22 = (-matriz.M02*sin(ang)) + (matriz.M22*cos(ang)); Temp.M23 = (-matriz.M03*sin(ang)) + (matriz.M23*cos(ang));

	matriz = Temp;
}

void RotacionYI_Matriz(Matriz_4x4 matriz, float ang)
{
	Matriz_4x4 Temp = matriz;
	Temp.M00 = (matriz.M00*cos(ang)) - (matriz.M20*sin(ang)); Temp.M01 = (matriz.M01*cos(ang)) - (matriz.M21*sin(ang));
	Temp.M02 = (matriz.M02*cos(ang)) - (matriz.M22*sin(ang)); Temp.M03 = (matriz.M03*cos(ang)) - (matriz.M23*sin(ang));
	Temp.M20 = (-matriz.M00*sin(ang)) + (matriz.M20*cos(ang)); Temp.M21 = (-matriz.M01*sin(ang)) + (matriz.M21*cos(ang));
	Temp.M22 = (-matriz.M02*sin(ang)) + (matriz.M22*cos(ang)); Temp.M23 = (-matriz.M03*sin(ang)) + (matriz.M23*cos(ang));

	matriz = Temp;
}
void RotacionZD_Matriz(Matriz_4x4 matriz, float ang)
{
	Matriz_4x4 Temp = matriz;
	Temp.M00 = (matriz.M00*cos(ang)) + (matriz.M10*sin(ang)); Temp.M01 = (matriz.M01*cos(ang)) + (matriz.M21*sin(ang));
	Temp.M02 = (matriz.M02*cos(ang)) + (matriz.M12*sin(ang)); Temp.M03 = (matriz.M03*cos(ang)) + (matriz.M23*sin(ang));
	Temp.M10 = (-matriz.M00*sin(ang)) + (matriz.M10*cos(ang)); Temp.M21 = (-matriz.M01*sin(ang)) + (matriz.M21*cos(ang));
	Temp.M12 = (-matriz.M02*sin(ang)) + (matriz.M12*cos(ang)); Temp.M23 = (-matriz.M03*sin(ang)) + (matriz.M23*cos(ang));

	matriz = Temp;
}
void RotacionZI_Matriz(Matriz_4x4 matriz, float ang)
{
	Matriz_4x4 Temp = matriz;
	Temp.M00 = (matriz.M00*cos(ang)) - (matriz.M10*sin(ang)); Temp.M01 = (matriz.M01*cos(ang)) + (matriz.M21*sin(ang));
	Temp.M02 = (matriz.M02*cos(ang)) - (matriz.M12*sin(ang)); Temp.M03 = (matriz.M03*cos(ang)) + (matriz.M23*sin(ang));
	Temp.M10 = (-matriz.M00*sin(ang)) + (matriz.M10*cos(ang)); Temp.M21 = (-matriz.M01*sin(ang)) + (matriz.M21*cos(ang));
	Temp.M12 = (-matriz.M02*sin(ang)) + (matriz.M12*cos(ang)); Temp.M23 = (-matriz.M03*sin(ang)) + (matriz.M23*cos(ang));

	matriz = Temp;
}
void Trasposicion_Matriz(Matriz_4x4 &matriz, Matriz_4x4 matriz2)
{
	matriz = matriz2;
	matriz.M01 = matriz2.M10; matriz.M02 = matriz2.M20; matriz.M03 = matriz2.M30;
	matriz.M10 = matriz2.M01; matriz.M12 = matriz2.M21; matriz.M13 = matriz2.M31;
	matriz.M20 = matriz2.M02; matriz.M21 = matriz2.M12; matriz.M23 = matriz2.M32;
	matriz.M30 = matriz2.M03; matriz.M31 = matriz2.M13; matriz.M32 = matriz2.M23;

}

void Imprimer_Vector3 (Vector3 &Vec3)
{
	std::cout << "( " << Vec3.X << " )" << std::endl;
	std::cout << "( " << Vec3.Y << " )" << std::endl;
	std::cout << "( " << Vec3.Z << " )" << std::endl;
}