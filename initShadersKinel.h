#ifndef INITSHADERS_H_
#define INITSHADERS_H_

#include "GL/glew.h"
#include "GL/freeglut.h"
#include "GL/gl.h"
#include "GL/glu.h"
#include <vector>
#include <cstdio>
#include <iostream>

using namespace std;

GLuint createShader(GLenum type, const GLchar* source);
const GLchar* inputShader(const char* filename);
GLuint createProgram(const vector<GLuint> list);

typedef struct{
  GLenum type;
  const char* filename;
} ShaderInfo;

//create the shaders
void initShaders(ShaderInfo* shaders){
  
  ShaderInfo* s=shaders;
  vector<GLuint> Shadelist;
  
  while(s->type != GL_NONE){
    //add shaders to list
    Shadelist.push_back(createShader(s->type,inputShader(s->filename)));
    ++s;
  }
  
  GLuint program=createProgram(Shadelist);
  glUseProgram(program);
}

//this funtion loads the shader from the vertex, fragments shaders 
const GLchar* inputShader(const char* filename){
  
  //opens the file
  FILE* fs = fopen(filename, "rb");
  
  //checks to see if the file opened
  if(!fs){
    fprintf(stderr,"ERR: Unable to open file '%s'\n",filename);
    return NULL;
  }
  
  //gets the length of the file
  fseek(fs, 0, SEEK_END);
  long size=ftell(fs);
  fseek(fs, 0, SEEK_SET);
  
  
  //reads in the file 
  GLchar* source = new GLchar[size+1];//
  fread(source, 1, size, fs);
  
  //checks to see if the file is empty
  if(ftell(fs) == 0){
    fprintf(stderr, "ERR: File '%s' is empty.\n",filename);
    return NULL;
  }

  //closes the file
  fclose(fs);
  
  //set '\0' at the end of the file
  source[size] = 0;
  
  //overloads the const  
  return const_cast<const GLchar*>(source);
}

//creates the shader
GLuint createShader(GLenum type, const GLchar* source){
  
  GLuint shader = glCreateShader(type);
  glShaderSource(shader, 1, &source, NULL);
  //compiles
  glCompileShader(shader);
  
  GLint compileStatus;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);
  
  //sees if compiled
  if(!compileStatus){
    GLint logSize;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logSize);
    
    GLchar* infoLog = new GLchar[logSize+1];
    glGetShaderInfoLog(shader,logSize,&logSize,infoLog);
    
    const char *info= NULL;
    switch(type){
      case GL_VERTEX_SHADER: info = "vertex"; break;
      case GL_GEOMETRY_SHADER_EXT: info = "geometric"; break;
      case GL_FRAGMENT_SHADER: info = "fragment"; break;
    }
    fprintf(stderr,"\nCompile failure in %u shader: %s\n Error message:\n%s\n",type,info,infoLog);
    delete[] infoLog;
  }
  return shader;
}

//creates the shading program
GLuint createProgram(const vector<GLuint> list){

  //creates the program
  GLuint myProgram = glCreateProgram();
  
  //attaches shaders to the program
  for(GLuint x=0;x<list.size();x++){
    glAttachShader(myProgram,list[x])
  ;}
  
  //binds location to the program
  glBindAttribLocation(myProgram, 0, "in_position");
  glBindAttribLocation(myProgram, 1, "in_color");
  //links program to program
  glLinkProgram(myProgram);
  
  GLint linkStatus;
  glGetProgramiv(myProgram, GL_LINK_STATUS, &linkStatus);
  
  //makes sure the program is linked to the program
  if(!linkStatus){
    GLint logSize;
    glGetProgramiv(myProgram, GL_INFO_LOG_LENGTH, &logSize);
    GLchar *log = new GLchar[logSize+1];
    glGetProgramInfoLog(myProgram,logSize,&logSize,log);
    
    fprintf(stderr,"\nShader linking failed: %s\n",log);
    delete[] log;
    
    for(GLuint x=0;x<list.size();x++){
      glDeleteShader(list[x]);
    }
  }
  return myProgram;
}
#endif
