#include "GL\glew.h"
#include "GL\glut.h"
#include <cstdlib>
#include <cassert>
#include <cmath>
#include <iostream>
#include <fstream>
#include <string>

static GLuint VBO;
//static GLuint gScaleLocation;
static GLuint gWorldLocation;
//static GLuint gColorLocation;

const char* pVertexShaderName = "vertex.shader";
const char* pFragmentShaderName = "fragment.shader";

class vector3f {
private:
	float x;
	float y;
	float z;
public:
	vector3f() {}
	vector3f(float _x, float _y, float _z) {
		x = _x;
		y = _y;
		z = _z;
	}
	~vector3f() {}
};

class matrix4f {
public:
	float m[4][4];
	matrix4f() {}
	~matrix4f() {}
};

extern bool ReadFile(const char* pFileName, std::string& outFile) {
	std::ifstream file(pFileName);
	bool ret = false;
	if (file.is_open()) {
		std::string line;
		while (!file.eof()) {
			std::getline(file, line);
			outFile.append(line);
			outFile.append("\n");
		}
		file.close();
		ret = true;
	}
	else {
		std::cout << stderr << "Error:  unable to open file"<< std::endl;
	}
	return ret;
}

static void InitGlew() {
	GLenum result = glewInit();
	if (result != GLEW_OK) {
		std::cout << stderr << "Error: " << glewGetErrorString(result) << std::endl;
	}
}

static void RenderTriangle() {
	glClear(GL_COLOR_BUFFER_BIT);
	static float scale = 0.0f;
	scale += 0.001f;
	//glUniform1f(gScaleLocation,sinf(scale));
	/*static matrix4f translation;
	translation.m[0][0] = 1.0f; translation.m[0][1] = 0.0f; translation.m[0][2] = 0.0f; translation.m[0][3] = sinf(scale);
	translation.m[1][0] = 0.0f; translation.m[1][1] = 1.0f; translation.m[1][2] = 0.0f; translation.m[1][3] = 0.0f;
	translation.m[2][0] = 0.0f; translation.m[2][1] = 0.0f; translation.m[2][2] = 1.0f; translation.m[2][3] = 0.0f;
	translation.m[3][0] = 0.0f; translation.m[3][1] = 0.0f; translation.m[3][2] = 0.0f; translation.m[3][3] = 1.0f;
	glUniformMatrix4fv(gWorldLocation, 1, GL_TRUE, &translation.m[0][0]);*/
	static matrix4f rotation;
	rotation.m[0][0] = cosf(scale); rotation.m[0][1] = 0.0f-sinf(scale); rotation.m[0][2] = 0.0f; rotation.m[0][3] = 0.0f;
	rotation.m[1][0] = sinf(scale); rotation.m[1][1] = cosf(scale); rotation.m[1][2] = 0.0f; rotation.m[1][3] = 0.0f;
	rotation.m[2][0] = 0.0f; rotation.m[2][1] = 0.0f; rotation.m[2][2] = 1.0f; rotation.m[2][3] = 0.0f;
	rotation.m[3][0] = 0.0f; rotation.m[3][1] = 0.0f; rotation.m[3][2] = 0.0f; rotation.m[3][3] = 1.0f;
	glUniformMatrix4fv(gWorldLocation, 1, GL_TRUE, &rotation.m[0][0]);
	/*static matrix4f scaling;
	scaling.m[0][0] = sinf(scale); scaling.m[0][1] = 0.0f; scaling.m[0][2] = 0.0f; scaling.m[0][3] = 0.0f;
	scaling.m[1][0] = 0.0f; scaling.m[1][1] = sinf(scale); scaling.m[1][2] = 0.0f; scaling.m[1][3] = 0.0f;
	scaling.m[2][0] = 0.0f; scaling.m[2][1] = 0.0f; scaling.m[2][2] = sinf(scale); scaling.m[2][3] = 0.0f;
	scaling.m[3][0] = 0.0f; scaling.m[3][1] = 0.0f; scaling.m[3][2] = 0.0f; scaling.m[3][3] = 1.0f;
	glUniformMatrix4fv(gWorldLocation,1,GL_TRUE,&scaling.m[0][0]);*/
	/*static float color = 0.0f;
	color += 0.01f;
	glUniform1f(gColorLocation, cosf(color));*/
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glDisableVertexAttribArray(0);
	glutSwapBuffers();
}

static void CreateVertexBuffer() {
	vector3f vertices[3];
	vertices[0] = vector3f(-0.5f, -0.5f, 0.0f);
	vertices[1] = vector3f(0.0f, 0.7f, 0.0f);
	vertices[2] = vector3f(0.5f, -0.5f, 0.0f);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}

static void AddShader(GLuint shaderProgram,const char* pShaderText,GLenum shaderType) {
	GLuint shaderObj = glCreateShader(shaderType);
	if (shaderObj == 0) {
		std::cout << "Error: Creating shader object failed" << std::endl;
		exit(1);
	}
	const GLchar* p[1];
	p[0] = pShaderText;
	GLint length[1];
	length[0] = strlen(pShaderText);
	glShaderSource(shaderObj, 1, p, length);
	glCompileShader(shaderObj);
	GLint is_success;
	glGetShaderiv(shaderObj,GL_COMPILE_STATUS,&is_success);
	if (!is_success) {
		GLchar infoLog[1024];
		glGetShaderInfoLog(shaderObj, 1024, NULL, infoLog);
		std::cout << stderr << "Error: Compile shader error" << infoLog << std::endl;
		exit(1);
	}
	glAttachShader(shaderProgram, shaderObj);
}

static void CompileShader() {
	GLuint ShaderProgram = glCreateProgram();
	if (ShaderProgram == 0) {
		std::cout << "Error: Creating shader program failed" << std::endl;
		exit(1);
	}
	std::string vs, fs;
	if (!ReadFile(pVertexShaderName, vs)) {
		exit(1);
	};
	if (!ReadFile(pFragmentShaderName, fs)) {
		exit(1);
	};
	AddShader(ShaderProgram, vs.c_str(), GL_VERTEX_SHADER);
	AddShader(ShaderProgram, fs.c_str(), GL_FRAGMENT_SHADER);
	GLint is_success = 0;
	glLinkProgram(ShaderProgram);
	glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &is_success);
	GLchar infoLog[1024];
	if (is_success == 0) {
		glGetProgramInfoLog(ShaderProgram,sizeof(infoLog),NULL,infoLog);
		std::cout << stderr << "Error: linking shader program failed\n" << infoLog << std::endl;
		exit(1);
	}
	glValidateProgram(ShaderProgram);
	glGetProgramiv(ShaderProgram,GL_VALIDATE_STATUS,&is_success);
	if (!is_success) {
		glGetProgramInfoLog(ShaderProgram, sizeof(infoLog), NULL, infoLog);
		std::cout << stderr << "Error: linking shader program failed\n" << infoLog << std::endl;
		exit(1);
	}
	glUseProgram(ShaderProgram);
	//gScaleLocation = glGetUniformLocation(ShaderProgram, "gScale");
	gWorldLocation = glGetUniformLocation(ShaderProgram, "gWorld");
	//gColorLocation = glGetUniformLocation(ShaderProgram, "gColor");
	assert(gWorldLocation != 0xFFFFFFFF);
	//assert(gScaleLocation != 0xFFFFFFFF && gColorLocation!=0xFFFFFFFF);
	//assert(gWorldLocation != 0xFFFFFFFF && gColorLocation != 0xFFFFFFFF);
}

static void InitializeGlutCallback() {
	glutDisplayFunc(RenderTriangle);
	glutIdleFunc(RenderTriangle);
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(50, 100);
	glutInitWindowSize(400, 300);
	glutCreateWindow("Render Triangle Using shader");

	InitializeGlutCallback();

	InitGlew();
	std::cout <<"GL version: "<<glGetString(GL_VERSION) << std::endl;

	glClearColor(0.0, 0.0, 0.0, 0.0);

	
	CreateVertexBuffer();

	CompileShader();

	glutMainLoop();
	return 0;
}