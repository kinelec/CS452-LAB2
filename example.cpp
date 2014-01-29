#include "initShaders.h"
using namespace std;
int counter=0;

GLuint vaoID, vboID;

GLfloat vertexarray[]={0.5f,0.5f,0.0f,
                       0.5f,-0.5f,0.0f,
                       -0.5f,-0.5f,0.0f,
                       -0.5f,0.5f,0.0f,
                       0.0f,0.5f,0.0f,
                       };
                       
GLfloat colorarray[]={1.0f,1.0f,0.0f,
                       0.0f,1.0f,0.0f,
                       1.0f,1.0f,1.0f,
                       0.5f,0.5f,1.0f,
                       1.0f,0.5f,0.0f,
                       };                       

void init(){

  glGenVertexArrays(1, &vaoID);
  glBindVertexArray(vaoID);
  
  glGenBuffers(1, &vboID);
  glBindBuffer(GL_ARRAY_BUFFER, vboID);
  glBufferData(GL_ARRAY_BUFFER,sizeof(vertexarray)+sizeof(colorarray),NULL,GL_STATIC_DRAW);
  glBufferSubData(GL_ARRAY_BUFFER,0,sizeof(vertexarray),vertexarray);
  glBufferSubData(GL_ARRAY_BUFFER,sizeof(vertexarray),sizeof(colorarray),colorarray);

 ShaderInfo shaders[]={
  { GL_VERTEX_SHADER , "vertexshader.glsl"} ,
  { GL_FRAGMENT_SHADER , "fragmentshader.glsl"},
  { GL_NONE , NULL} 
  };

  initShaders(shaders);
  
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,vertexarray);
  glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,colorarray);
}

void drawscene(){
  glClear(GL_COLOR_BUFFER_BIT);
  glDrawArrays(GL_TRIANGLES,0,3);
  glFlush();
}

void mousepress(int button, int state, int x, int y){
  if(button==GLUT_RIGHT_BUTTON && state==GLUT_DOWN)
    exit(0);
  else if(button==GLUT_LEFT_BUTTON && state==GLUT_DOWN){
    counter++;
    cout << counter%3 << endl;
    glutDisplayFunc(drawscene);
  }
}


int main(int argc,char ** argv){

  glutInit(&argc, argv);
  glutCreateWindow("Shapes");//creates the window with the specified name
  
  //initializes glew
  glewExperimental=GL_TRUE;
  if(glewInit()){
    fprintf(stderr, "Unable to initalize GLEW");
    exit(EXIT_FAILURE);
  }
  
  glutInitContextVersion(4, 3);//specifies the version of opengl
  glutInitContextProfile(GLUT_CORE_PROFILE|GLUT_COMPATIBILITY_PROFILE);//specifies what profile your using


  //retruns what version of opengl and glsl your computer can use
  const GLubyte* version=glGetString(GL_SHADING_LANGUAGE_VERSION);
  fprintf(stderr,"Opengl glsl version %s\n", version);

  version=glGetString(GL_VERSION);
  fprintf(stderr,"Opengl version %s\n", version);

  glutDisplayFunc(drawscene);//displays callback draws the shapes
  glutMouseFunc(mousepress);//control callback specifies the mouse controls
  glutMainLoop();//sets opengl state in a neverending loop
  return 0;
}
