#define GL_GLEXT_PROTOTYPES

#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>
//#include <GL/freeglut.h>
#include "render.h"
#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"
#include "../glm/gtc/type_ptr.hpp"


GLfloat xRotated, yRotated, zRotated;
// Cone
// GLdouble base=3;
// GLdouble height=3.5;
// GLint slices =50;
// GLint stacks =50;



// void displayCone(void)
// {

//     glMatrixMode(GL_MODELVIEW);
//     // clear the drawing buffer.
//     glClear(GL_COLOR_BUFFER_BIT);
//     // clear the identity matrix.
//     glLoadIdentity();
//     // traslate the draw by z = -4.0
//     // Note this when you decrease z like -8.0 the drawing will looks far , or smaller.
//     glTranslatef(0.0,0.0,-4.5);
//     // Red color used to draw.
//     glColor3f(0.8, 0.2, 0.1); 
//     // changing in transformation matrix.
//     // rotation about X axis
//     glRotatef(xRotated,1.0,0.0,0.0);
//     // rotation about Y axis
//     glRotatef(yRotated,0.0,1.0,0.0);
//     // rotation about Z axis
//     glRotatef(zRotated,0.0,0.0,1.0);
//     // scaling transfomation 
//     glScalef(1.0,1.0,1.0);
//     // built-in (glut library) function , draw you a Cone.
 

//     glutSolidCone(base,height,slices,stacks);
//     // Flush buffers to screen
     
//     glFlush();        
//     // sawp buffers called because we are using double buffering 
//    // glutSwapBuffers();
// }




// void reshapeCone(int x, int y)
// {
//     if (y == 0 || x == 0) return;  //Nothing is visible then, so return
//     //Set a new projection matrix
//     glMatrixMode(GL_PROJECTION);  
//     glLoadIdentity();
//     //Angle of view:40 degrees
//     //Near clipping plane distance: 0.5
//     //Far clipping plane distance: 20.0
     
//     gluPerspective(40.0,(GLdouble)x/(GLdouble)y,0.5,20.0);
 
//     glViewport(0,0,x,y);  //Use the whole window for rendering
// }

// void idleCone(void)
// {
 
//      yRotated += 0.01;
     
//     displayCone();
// }




GLuint vbo[4];		//identyfikatory buforow wierzcholkow
GLuint vao[2];		//identyfikatory tablic wierzcholkow
GLuint ebo;		//identyfikator bufora elementow

GLuint shaderProgram;
GLint vertexShader;	//identyfikator shadera wierzcholkow
GLint fragmentShader;   //identyfikator shadera fragmentow
GLint posAttrib, colAttrib;	//

glm::mat4 viewMatrix = glm::mat4();
glm::mat4 projectionMatrix = glm::mat4(); //marzerz widoku i rzutowania
GLfloat fi = 0;


//-------------Atrybuty wierzcholkow------------------------------------------

    // draw cone using 9 vertices
	GLfloat ver_triangle[] = {
        0.0f,  1.0f, 0.0, // 1

        0.0f,  0.0f, -1.0, // 2
        0.5f,  0.0f, -0.7, // 3
        0.7f,  0.0f, 0.0, // 4
        0.5f,  0.0f, 0.7, // 5

        0.0f,  0.0f, 1.0, // 6 | 2
        -0.5f,  0.0f, 0.7, // 7 | 3
        -0.7f,  0.0f, 0.0, // 8 | 4
        -0.5f,  0.0f, -0.7, // 9 | 5



        // 0.0f,  0.0f, 1.0,
        // 0.0f,  0.0f, -1.0,
        
        
        
        // 0.0f, 0.0f, 0.0f, // w0
        // 0.0f, 0.0f, -1.0f, // w1
        // 0.5f, 0.0f, -1.0f, // w2
        // 0.5f, 0.0f, -1.0f, // w3
        // 0.5f, 0.0f, -1.0f, // w4
        // 0.5f, 0.0f, -1.0f, // w5
        // 0.5f, 0.0f, -1.0f, // w6
        // 0.5f, 0.0f, -1.0f, // w7
	};


        
    
        // -1.0f,  0.0f, -1.0,
        // 1.0f,  0.0f, -1.0,

	GLfloat col_triangle[] = {	//kolory wierzcholkow trojkata
		1.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
	};

  // XYZ
	GLfloat ver_rectangle[] = {	//wspolrzedne wierzcholkow prostokata
		-1.0f, -0.2f, 0.0f, // w0
		 1.0f, -0.2f, 0.0f, // w1
		-1.0f, -0.7f, 0.0f, // w2 - 1. trójkąt
		 1.0f, -0.7f, 0.0f, // w3 - 2. trójkąt
		 1.0f, -0.2f, -1.0f, // w4 - 3. trójkąt - prawy bok
		 1.0f, -0.7f, -1.0f, // w5 - 4. trójkąt - prawy bok
		 -1.0f, -0.7f, -1.0f, // w6 - 5. trójkąt - lewy bok
		 -1.0f, -0.2f, -1.0f, // w7 - 6. trójkąt - lewy bok
	};

	GLfloat col_rectangle[] = {	//kolory wierzcholkow prostokata
		0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f, // 1.
		0.0f, 1.0f, 0.0f, // 2.
		0.0f, 1.0f, 0.0f, // 3.
		0.0f, 1.0f, 0.0f, // 4.
		0.0f, 1.0f, 0.0f, // 5.
		0.0f, 1.0f, 0.0f, // 6.
		0.0f, 1.0f, 0.0f, // 7.
		0.0f, 1.0f, 0.0f, // 8.
		1.0f, 1.0f, 0.8f, // 9.
		2.0f, 1.0f, 0.5f, // 10.
		1.0f, 1.0f, 0.0f, // 11.
	};

	GLuint elements[] = { //prostokat skladamy z dwoch trojkatow
		0, 1, 2,		  //indeksy wierzcholkow dla pierwszego trojkata
		1, 2, 3,			  //indeksy wierzcholkow dla drugiego trojkata
		1, 3, 4,  // 3.
		3, 4, 5,  // 4.
		3, 4, 5,  // 5.
		0, 7, 6,  // 6.
		0, 2, 6,  // 7.
		7, 6, 5,  // 8.
		7, 5, 4,  // 9.

    // pokrywa góra
		0, 1, 7,  // 10.
		1, 4, 7,  // 11.
	};
	

//----------------------------kod shadera wierzcholkow-----------------------------------------

const GLchar* vShader_string =
{
  "#version 130\n"\

  "in vec3 position;\n"\
  "in vec3 color;\n"\
  "out vec3 Color;\n"\
  "uniform mat4 transformMatrix;\n"\
  "void main(void)\n"\
  "{\n"\
  "  gl_Position = transformMatrix * vec4(position, 1.0);\n"\
  "  Color = color;\n"\
  "}\n"
};

//----------------------------kod shadera fragmentow-------------------------------------------
const GLchar* fShader_string =
{
  "#version 130\n"\
  "in  vec3 Color;\n"\
  "out vec4 outColor;\n"\

  "void main(void)\n"\
  "{\n"\
  "  outColor = vec4(Color, 1.0);\n"\
  "}\n"
};


//------------------------------------------------zmiana rozmiaru okna---------------------------

void resizeGLScene(unsigned int width, unsigned int height)
{
    if (height == 0) height = 1; // zabezpiecznie dla okna o zerowej wysokosci
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION); 
    glLoadIdentity();
    gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 1.0f, 500.0f);
    glMatrixMode(GL_MODELVIEW);
}


//----------------------------------tworzenie, wczytanie, kompilacja shaderow-------------------------

int initShaders(void)
{
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vShader_string, NULL);
    glCompileShader(vertexShader);
    
    GLint status;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
    if (status == GL_TRUE)
      std::cout << "Kompilacja shadera wierzcholkow powiodla sie!\n";
    else
    {
      std::cout << "Kompilacja shadera wierzcholkow NIE powiodla sie!\n";
      return 0;
     }
     
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fShader_string, NULL); 
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);
    if (status == GL_TRUE)
      std::cout << "Kompilacja shadera fragmentow powiodla sie!\n";
    else
    {
      std::cout << "Kompilacja shadera fragmentow NIE powiodla sie!\n";
      return 0;
    }

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    
    //glBindFragDataLocation(shaderProgram, 0, "outColor"); 

    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);
    return 1;
}



//--------------------------------------------funkcja inicjujaca-------------------------------------
int initGL(void)
{
   
    if(initShaders())
    {   

        glEnable(GL_DEPTH_TEST);
	    glDepthFunc(GL_LEQUAL); 
	    glGenVertexArrays(2, vao); //przypisanie do vao identyfikatorow tablic
	    glGenBuffers(4, vbo);	   //przypisanie do vbo identyfikatorow buforow
	    glGenBuffers(1, &ebo);

	    posAttrib = glGetAttribLocation(shaderProgram, "position"); //pobranie indeksu tablicy atrybutow wierzcholkow okreslajacych polozenie
        glEnableVertexAttribArray(posAttrib);
	    colAttrib = glGetAttribLocation(shaderProgram, "color");    //pobranie indeksu tablicy atrybutow wierzcholkow okreslajacych kolor
        glEnableVertexAttribArray(colAttrib);
	
        glBindVertexArray(vao[0]);					//wybor tablicy
            
        glBindBuffer(GL_ARRAY_BUFFER, vbo[0]); 							//powiazanie bufora z odpowiednim obiektem (wybor bufora) 
        glBufferData(GL_ARRAY_BUFFER, sizeof(ver_triangle), ver_triangle, GL_STATIC_DRAW); 	//skopiowanie danych do pamieci aktywnego bufora
        glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);				//okreslenie organizacji danych w tablicy wierzcholkow
        glEnableVertexAttribArray(posAttrib);							//wlaczenie tablicy
        
        glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(col_triangle), col_triangle, GL_STATIC_DRAW);
        glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(colAttrib);
        
        glBindVertexArray(vao[1]);

        glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(ver_rectangle), ver_rectangle, GL_STATIC_DRAW);
        glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(posAttrib);

        glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(col_rectangle), col_rectangle, GL_STATIC_DRAW);
        glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(colAttrib);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);


            
        //macierz widoku (okresla polozenie kamery i kierunek w ktorym jest skierowana) 
        viewMatrix = glm::lookAt(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f)); 
        //macierz rzutowania perspektywicznego
        projectionMatrix = glm::perspective(glm::radians(45.0f), 1.0f, 1.0f, 10.0f);		

        return 1;
    }

    else
	return 0;
}

//------------------------------------------renderowanie sceny-------------------------------------

int drawGLScene(int counter)
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    
    glm::mat4 translationMatrix = glm::translate(glm::mat4(), glm::vec3(0.0f, 0.0f, 0.0f));  		//macierz przesuniecia o zadany wektor
    glm::mat4 rotationMatrix = glm::rotate(glm::mat4(), glm::radians(fi), glm::vec3(0.0f, 1.0f, 0.0f)); //macierz obrotu o dany kat wokol wektora
		
    glm::mat4 transformMatrix = projectionMatrix * viewMatrix * translationMatrix * rotationMatrix; //wygenerowanie macierzy uwzgledniajacej wszystkie transformacje


    GLint transformMatrixUniformLocation = glGetUniformLocation(shaderProgram, "transformMatrix");  //pobranie polozenia macierzy bedacej zmienna jednorodna shadera
    glUniformMatrix4fv(transformMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(transformMatrix)); //zapisanie macierzy bedacej zmienna jednorodna shadera wierzcholkow
    

    glBindVertexArray(vao[0]);
    // glDrawArrays(GL_TRIANGLES, 0, 3); //rysowanie trojkata
    glDrawArrays(GL_TRIANGLE_FAN, 0, 9); //rysowanie stożka


    // ZMIENIAĆ ILOŚĆ WIERZCHOŁKÓW
    glBindVertexArray(vao[1]);
    glDrawElements(GL_TRIANGLES, 33, GL_UNSIGNED_INT, 0); //rysowanie prostokata




    fi += 0.5;
 
    glFlush();

    return 1;    
}

//----------------------------------------------------porzadki--------------------------------------

void deleteAll()
{
    glDeleteProgram(shaderProgram);
    glDeleteShader(fragmentShader);
    glDeleteShader(vertexShader);

    glDeleteBuffers(4, vbo);
    glDeleteBuffers(1, &ebo);
    glDeleteVertexArrays(2, vao);
}
