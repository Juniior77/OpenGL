#include <jni.h>
#include "GL4D/gl4droid.h"

GLuint _pId, _posHandle, _colorHandle;

void init(const char * vs, const char * fs) {
    glClearColor(0, 0, 0, 1);
    gl4duGenMatrix(GL_FLOAT, "modelViewMatrix");
    gl4duGenMatrix(GL_FLOAT, "projectionMatrix");
    if(!(_pId = gl4droidCreateProgram(vs, fs))) return;
    _posHandle = glGetAttribLocation(_pId, "vPosition");
    _colorHandle = glGetAttribLocation(_pId, "vColor");
    return;
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    gl4duBindMatrix("projectionMatrix");
    gl4duLoadIdentityf();
    gl4duFrustumf(-0.5, 0.5, -0.5 * h / w, 0.5 * h / w, 1.5, 1000.0);
    gl4duBindMatrix("modelViewMatrix");
}

void drawold(void) {
    static GLfloat a0 = 0.0f;
   /* GLfloat data[] = {
            // 4 vertices
            -0.9, -0.9, -0.1, -0.9, -0.9, -0.1, -0.1, -0.1,
             0.1, 0.1, 0.1, 0.9, 0.9, 0.1, 0.9, 0.9,
             -0.1, 0.1, -0.9, 0.1, -0.9, 0.9, -0.1, 0.9,
             0.1, -0.1, 0.9, -0.1, 0.9, -0.9, 0.1, -0.9,
            // 4 RGB colors
            1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0,
            1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0,
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
    };*/
    GLfloat data[100];
    GLfloat x_orig = -1;
    GLfloat y_orig = -1;
    GLfloat x_ratio = 0.2;
    GLfloat y_ratio = 0.2;
    GLfloat x;
    GLfloat y;
    for(int i = 0; i < 100; i+8)
    {
        x = x_orig + (i*x_ratio);
        y = y_orig + (i*y_ratio);
        data[i] = x;
        data[i+1] = y;
        data[i+2] = x + x_ratio;
        data[i+3] = y;
        data[i+4] = x;
        data[i+5] = y + y_ratio;
        data[i+6] = x + x_ratio;
        data[i+7] = y + y_ratio;
    }

    GLfloat color[1200] = {
            // 4 RGB colors
            1, 0, 1,    1, 0, 1,    1, 0, 1,    1, 0, 1,
            1, 0, 0,    1, 0, 0,    1, 0, 0,    1, 0, 0,
            1, 1, 0,    1, 1, 0,    1, 1, 0,    1, 1, 0,
            0, 1, 0,    0, 1, 0,    0, 1, 0,    0, 1, 0,
            0, 0, 1,    0, 0, 1,    0, 0, 1,    0, 0, 1,
            0, 1, 1,    0, 1, 1,    0, 1, 1,    0, 1, 1,
            0, 0, 0,    0, 0, 0,    0, 0, 0,    0, 0, 0,
            1, 0, 1,    1, 0, 1,    1, 0, 1,    1, 0, 1,
            1, 0, 0,    1, 0, 0,    1, 0, 0,    1, 0, 0,
            1, 1, 0,    1, 1, 0,    1, 1, 0,    1, 1, 0,
            0, 1, 0,    0, 1, 0,    0, 1, 0,    0, 1, 0,
            0, 0, 1,    0, 0, 1,    0, 0, 1,    0, 0, 1,
            0, 1, 1,    0, 1, 1,    0, 1, 1,    0, 1, 1,
            0, 0, 0,    0, 0, 0,    0, 0, 0,    0, 0, 0,
            1, 0, 1,    1, 0, 1,    1, 0, 1,    1, 0, 1,
            1, 0, 0,    1, 0, 0,    1, 0, 0,    1, 0, 0,
            1, 1, 0,    1, 1, 0,    1, 1, 0,    1, 1, 0,
            0, 1, 0,    0, 1, 0,    0, 1, 0,    0, 1, 0,
            0, 0, 1,    0, 0, 1,    0, 0, 1,    0, 0, 1,
            0, 1, 1,    0, 1, 1,    0, 1, 1,    0, 1, 1,
            0, 0, 0,    0, 0, 0,    0, 0, 0,    0, 0, 0,
            1, 0, 1,    1, 0, 1,    1, 0, 1,    1, 0, 1,
            1, 0, 0,    1, 0, 0,    1, 0, 0,    1, 0, 0,
            1, 1, 0,    1, 1, 0,    1, 1, 0,    1, 1, 0,
            0, 1, 0,    0, 1, 0,    0, 1, 0,    0, 1, 0,
            0, 0, 1,    0, 0, 1,    0, 0, 1,    0, 0, 1,
            0, 1, 1,    0, 1, 1,    0, 1, 1,    0, 1, 1,
            0, 0, 0,    0, 0, 0,    0, 0, 0,    0, 0, 0,
            1, 0, 1,    1, 0, 1,    1, 0, 1,    1, 0, 1,
            1, 0, 0,    1, 0, 0,    1, 0, 0,    1, 0, 0,
            1, 1, 0,    1, 1, 0,    1, 1, 0,    1, 1, 0,
            0, 1, 0,    0, 1, 0,    0, 1, 0,    0, 1, 0,
            0, 0, 1,    0, 0, 1,    0, 0, 1,    0, 0, 1,
            0, 1, 1,    0, 1, 1,    0, 1, 1,    0, 1, 1,
            0, 0, 0,    0, 0, 0,    0, 0, 0,    0, 0, 0,
            1, 0, 1,    1, 0, 1,    1, 0, 1,    1, 0, 1,
            1, 0, 0,    1, 0, 0,    1, 0, 0,    1, 0, 0,
            1, 1, 0,    1, 1, 0,    1, 1, 0,    1, 1, 0,
            0, 1, 0,    0, 1, 0,    0, 1, 0,    0, 1, 0,
            0, 0, 1,    0, 0, 1,    0, 0, 1,    0, 0, 1,
            0, 1, 1,    0, 1, 1,    0, 1, 1,    0, 1, 1,
            0, 0, 0,    0, 0, 0,    0, 0, 0,    0, 0, 0,
            1, 0, 1,    1, 0, 1,    1, 0, 1,    1, 0, 1,
            1, 0, 0,    1, 0, 0,    1, 0, 0,    1, 0, 0,
            1, 1, 0,    1, 1, 0,    1, 1, 0,    1, 1, 0,
            0, 1, 0,    0, 1, 0,    0, 1, 0,    0, 1, 0,
            0, 0, 1,    0, 0, 1,    0, 0, 1,    0, 0, 1,
            0, 1, 1,    0, 1, 1,    0, 1, 1,    0, 1, 1,
            0, 0, 0,    0, 0, 0,    0, 0, 0,    0, 0, 0,
            1, 0, 1,    1, 0, 1,    1, 0, 1,    1, 0, 1,
            1, 0, 0,    1, 0, 0,    1, 0, 0,    1, 0, 0,
            1, 1, 0,    1, 1, 0,    1, 1, 0,    1, 1, 0,
            0, 1, 0,    0, 1, 0,    0, 1, 0,    0, 1, 0,
            0, 0, 1,    0, 0, 1,    0, 0, 1,    0, 0, 1,
            0, 1, 1,    0, 1, 1,    0, 1, 1,    0, 1, 1,
            0, 0, 0,    0, 0, 0,    0, 0, 0,    0, 0, 0,
            1, 0, 1,    1, 0, 1,    1, 0, 1,    1, 0, 1,
            1, 0, 0,    1, 0, 0,    1, 0, 0,    1, 0, 0,
            1, 1, 0,    1, 1, 0,    1, 1, 0,    1, 1, 0,
            0, 1, 0,    0, 1, 0,    0, 1, 0,    0, 1, 0,
            0, 0, 1,    0, 0, 1,    0, 0, 1,    0, 0, 1,
            0, 1, 1,    0, 1, 1,    0, 1, 1,    0, 1, 1,
            0, 0, 0,    0, 0, 0,    0, 0, 0,    0, 0, 0,
            1, 0, 1,    1, 0, 1,    1, 0, 1,    1, 0, 1,
            1, 0, 0,    1, 0, 0,    1, 0, 0,    1, 0, 0,
            1, 1, 0,    1, 1, 0,    1, 1, 0,    1, 1, 0,
            0, 1, 0,    0, 1, 0,    0, 1, 0,    0, 1, 0,
            0, 0, 1,    0, 0, 1,    0, 0, 1,    0, 0, 1,
            0, 1, 1,    0, 1, 1,    0, 1, 1,    0, 1, 1,
            0, 0, 0,    0, 0, 0,    0, 0, 0,    0, 0, 0,
            1, 0, 1,    1, 0, 1,    1, 0, 1,    1, 0, 1,
            1, 0, 0,    1, 0, 0,    1, 0, 0,    1, 0, 0,
            1, 1, 0,    1, 1, 0,    1, 1, 0,    1, 1, 0,
            0, 1, 0,    0, 1, 0,    0, 1, 0,    0, 1, 0,
            0, 0, 1,    0, 0, 1,    0, 0, 1,    0, 0, 1,
            0, 1, 1,    0, 1, 1,    0, 1, 1,    0, 1, 1,
            0, 0, 0,    0, 0, 0,    0, 0, 0,    0, 0, 0,
            1, 0, 1,    1, 0, 1,    1, 0, 1,    1, 0, 1,
            1, 0, 0,    1, 0, 0,    1, 0, 0,    1, 0, 0,
            1, 1, 0,    1, 1, 0,    1, 1, 0,    1, 1, 0,
            0, 1, 0,    0, 1, 0,    0, 1, 0,    0, 1, 0,
            0, 0, 1,    0, 0, 1,    0, 0, 1,    0, 0, 1,
            0, 1, 1,    0, 1, 1,    0, 1, 1,    0, 1, 1,
            0, 0, 0,    0, 0, 0,    0, 0, 0,    0, 0, 0,
            1, 0, 1,    1, 0, 1,    1, 0, 1,    1, 0, 1,
            1, 0, 0,    1, 0, 0,    1, 0, 0,    1, 0, 0,
            1, 1, 0,    1, 1, 0,    1, 1, 0,    1, 1, 0,
            0, 1, 0,    0, 1, 0,    0, 1, 0,    0, 1, 0,
            0, 0, 1,    0, 0, 1,    0, 0, 1,    0, 0, 1,
            0, 1, 1,    0, 1, 1,    0, 1, 1,    0, 1, 1,
            0, 0, 0,    0, 0, 0,    0, 0, 0,    0, 0, 0,
            1, 0, 1,    1, 0, 1,    1, 0, 1,    1, 0, 1,
            1, 0, 0,    1, 0, 0,    1, 0, 0,    1, 0, 0,
            1, 1, 0,    1, 1, 0,    1, 1, 0,    1, 1, 0,
            0, 1, 0,    0, 1, 0,    0, 1, 0,    0, 1, 0,
            0, 0, 1,    0, 0, 1,    0, 0, 1,    0, 0, 1,
            0, 1, 1,    0, 1, 1,    0, 1, 1,    0, 1, 1,
            0, 0, 0,    0, 0, 0,    0, 0, 0,    0, 0, 0,
            1, 0, 1,    1, 0, 1,    1, 0, 1,    1, 0, 1,
            1, 0, 0,    1, 0, 0,    1, 0, 0,    1, 0, 0
    };

    int o;
    GLubyte indices[10*10*4];

    for (o = 0 ; o < (10*10*4) ; o++){
        indices[o] = (GLubyte) o;
    }

    glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glUseProgram(_pId);

    glUniform1f(glGetUniformLocation(_pId,"ID" ), 0);
    glVertexAttribPointer(_posHandle, 2, GL_FLOAT, GL_FALSE, 0, data);
    glEnableVertexAttribArray(_posHandle);
    glVertexAttribPointer(_colorHandle, 3, GL_FLOAT, GL_FALSE, 0, &data[32]);
    glEnableVertexAttribArray(_colorHandle);
    gl4duBindMatrix("modelViewMatrix");
    gl4duLoadIdentityf();
    gl4duTranslatef(0, 0, -6);
    gl4duSendMatrices();

    GLubyte ind[4];
    for (int i = 0 ; i < 100 ; i++) {
        for (int j = 0; j < 4; j++) {
            ind[j] = indices[(j * 4) + j];
        }
        glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, ind);
    }
}

void draw(void){
    static GLfloat a0 = 90.0f;


    GLfloat position[14][80];
    int i, j;
    GLfloat x_orig = -1;
    GLfloat y_orig = -1;
    GLfloat ratio_x = 0.2;
    GLfloat ratio_y = 0.2;
    for (i = 0 ; i < 14 ; i++){
        GLfloat x = x_orig + (i * ratio_x);
        for (j = 0; j < 10 ; j++){
            GLfloat y = y_orig + (j * ratio_y);
            position[i][(j * 8) + 0] = x;
            position[i][(j * 8) + 1] = y;
            position[i][(j * 8) + 2] = x + ratio_x;
            position[i][(j * 8) + 3] = y;
            position[i][(j * 8) + 4] = x;
            position[i][(j * 8) + 5] = y + ratio_y;
            position[i][(j * 8) + 6] = x + ratio_x;
            position[i][(j * 8) + 7] = y + ratio_y;
        }
    }
    GLfloat color[140*12] = {
            // 4 RGB colors
            1, 1, 1,    1, 0, 1,    1, 0, 1,    1, 0, 1,
            1, 0, 0,    1, 0, 0,    1, 0, 0,    1, 0, 0,
            1, 1, 0,    1, 1, 0,    1, 1, 0,    1, 1, 0,
            0, 1, 0,    0, 1, 0,    0, 1, 0,    0, 1, 0,
            0, 0, 1,    0, 0, 1,    0, 0, 1,    0, 0, 1,
            0, 1, 1,    0, 1, 1,    0, 1, 1,    0, 1, 1,
            0, 0, 0,    0, 0, 0,    0, 0, 0,    0, 0, 0,
            1, 0, 1,    1, 0, 1,    1, 0, 1,    1, 0, 1,
            1, 0, 0,    1, 0, 0,    1, 0, 0,    1, 0, 0,
            1, 1, 0,    1, 1, 0,    1, 1, 0,    1, 1, 0,
            0, 1, 0,    0, 1, 0,    0, 1, 0,    0, 1, 0,
            0, 0, 1,    0, 0, 1,    0, 0, 1,    0, 0, 1,
            0, 1, 1,    0, 1, 1,    0, 1, 1,    0, 1, 1,
            0, 0, 0,    0, 0, 0,    0, 0, 0,    0, 0, 0,
            1, 0, 1,    1, 0, 1,    1, 0, 1,    1, 0, 1,
            1, 0, 0,    1, 0, 0,    1, 0, 0,    1, 0, 0,
            1, 1, 0,    1, 1, 0,    1, 1, 0,    1, 1, 0,
            0, 1, 0,    0, 1, 0,    0, 1, 0,    0, 1, 0,
            0, 0, 1,    0, 0, 1,    0, 0, 1,    0, 0, 1,
            0, 1, 1,    0, 1, 1,    0, 1, 1,    0, 1, 1,
            0, 0, 0,    0, 0, 0,    0, 0, 0,    0, 0, 0,
            1, 0, 1,    1, 0, 1,    1, 0, 1,    1, 0, 1,
            1, 0, 0,    1, 0, 0,    1, 0, 0,    1, 0, 0,
            1, 1, 0,    1, 1, 0,    1, 1, 0,    1, 1, 0,
            0, 1, 0,    0, 1, 0,    0, 1, 0,    0, 1, 0,
            0, 0, 1,    0, 0, 1,    0, 0, 1,    0, 0, 1,
            0, 1, 1,    0, 1, 1,    0, 1, 1,    0, 1, 1,
            0, 0, 0,    0, 0, 0,    0, 0, 0,    0, 0, 0,
            1, 0, 1,    1, 0, 1,    1, 0, 1,    1, 0, 1,
            1, 0, 0,    1, 0, 0,    1, 0, 0,    1, 0, 0,
            1, 1, 0,    1, 1, 0,    1, 1, 0,    1, 1, 0,
            0, 1, 0,    0, 1, 0,    0, 1, 0,    0, 1, 0,
            0, 0, 1,    0, 0, 1,    0, 0, 1,    0, 0, 1,
            0, 1, 1,    0, 1, 1,    0, 1, 1,    0, 1, 1,
            0, 0, 0,    0, 0, 0,    0, 0, 0,    0, 0, 0,
            1, 0, 1,    1, 0, 1,    1, 0, 1,    1, 0, 1,
            1, 0, 0,    1, 0, 0,    1, 0, 0,    1, 0, 0,
            1, 1, 0,    1, 1, 0,    1, 1, 0,    1, 1, 0,
            0, 1, 0,    0, 1, 0,    0, 1, 0,    0, 1, 0,
            0, 0, 1,    0, 0, 1,    0, 0, 1,    0, 0, 1,
            0, 1, 1,    0, 1, 1,    0, 1, 1,    0, 1, 1,
            0, 0, 0,    0, 0, 0,    0, 0, 0,    0, 0, 0,
            1, 0, 1,    1, 0, 1,    1, 0, 1,    1, 0, 1,
            1, 0, 0,    1, 0, 0,    1, 0, 0,    1, 0, 0,
            1, 1, 0,    1, 1, 0,    1, 1, 0,    1, 1, 0,
            0, 1, 0,    0, 1, 0,    0, 1, 0,    0, 1, 0,
            0, 0, 1,    0, 0, 1,    0, 0, 1,    0, 0, 1,
            0, 1, 1,    0, 1, 1,    0, 1, 1,    0, 1, 1,
            0, 0, 0,    0, 0, 0,    0, 0, 0,    0, 0, 0,
            1, 0, 1,    1, 0, 1,    1, 0, 1,    1, 0, 1,
            1, 0, 0,    1, 0, 0,    1, 0, 0,    1, 0, 0,
            1, 1, 0,    1, 1, 0,    1, 1, 0,    1, 1, 0,
            0, 1, 0,    0, 1, 0,    0, 1, 0,    0, 1, 0,
            0, 0, 1,    0, 0, 1,    0, 0, 1,    0, 0, 1,
            0, 1, 1,    0, 1, 1,    0, 1, 1,    0, 1, 1,
            0, 0, 0,    0, 0, 0,    0, 0, 0,    0, 0, 0,
            1, 0, 1,    1, 0, 1,    1, 0, 1,    1, 0, 1,
            1, 0, 0,    1, 0, 0,    1, 0, 0,    1, 0, 0,
            1, 1, 0,    1, 1, 0,    1, 1, 0,    1, 1, 0,
            0, 1, 0,    0, 1, 0,    0, 1, 0,    0, 1, 0,
            0, 0, 1,    0, 0, 1,    0, 0, 1,    0, 0, 1,
            0, 1, 1,    0, 1, 1,    0, 1, 1,    0, 1, 1,
            0, 0, 0,    0, 0, 0,    0, 0, 0,    0, 0, 0,
            1, 0, 1,    1, 0, 1,    1, 0, 1,    1, 0, 1,
            1, 0, 0,    1, 0, 0,    1, 0, 0,    1, 0, 0,
            1, 1, 0,    1, 1, 0,    1, 1, 0,    1, 1, 0,
            0, 1, 0,    0, 1, 0,    0, 1, 0,    0, 1, 0,
            0, 0, 1,    0, 0, 1,    0, 0, 1,    0, 0, 1,
            0, 1, 1,    0, 1, 1,    0, 1, 1,    0, 1, 1,
            0, 0, 0,    0, 0, 0,    0, 0, 0,    0, 0, 0,
            1, 0, 1,    1, 0, 1,    1, 0, 1,    1, 0, 1,
            1, 0, 0,    1, 0, 0,    1, 0, 0,    1, 0, 0,
            1, 1, 0,    1, 1, 0,    1, 1, 0,    1, 1, 0,
            0, 1, 0,    0, 1, 0,    0, 1, 0,    0, 1, 0,
            0, 0, 1,    0, 0, 1,    0, 0, 1,    0, 0, 1,
            0, 1, 1,    0, 1, 1,    0, 1, 1,    0, 1, 1,
            0, 0, 0,    0, 0, 0,    0, 0, 0,    0, 0, 0,
            1, 0, 1,    1, 0, 1,    1, 0, 1,    1, 0, 1,
            1, 0, 0,    1, 0, 0,    1, 0, 0,    1, 0, 0,
            1, 1, 0,    1, 1, 0,    1, 1, 0,    1, 1, 0,
            0, 1, 0,    0, 1, 0,    0, 1, 0,    0, 1, 0,
            0, 0, 1,    0, 0, 1,    0, 0, 1,    0, 0, 1,
            0, 1, 1,    0, 1, 1,    0, 1, 1,    0, 1, 1,
            1, 0, 1,    1, 0, 1,    1, 0, 1,    1, 0, 1,
            1, 0, 0,    1, 0, 0,    1, 0, 0,    1, 0, 0,
            1, 1, 0,    1, 1, 0,    1, 1, 0,    1, 1, 0,
            0, 1, 0,    0, 1, 0,    0, 1, 0,    0, 1, 0,
            0, 0, 1,    0, 0, 1,    0, 0, 1,    0, 0, 1,
            0, 1, 1,    0, 1, 1,    0, 1, 1,    0, 1, 1,
            0, 0, 0,    0, 0, 0,    0, 0, 0,    0, 0, 0,
            1, 0, 1,    1, 0, 1,    1, 0, 1,    1, 0, 1,
            1, 0, 0,    1, 0, 0,    1, 0, 0,    1, 0, 0,
            1, 1, 0,    1, 1, 0,    1, 1, 0,    1, 1, 0,
            0, 1, 0,    0, 1, 0,    0, 1, 0,    0, 1, 0,
            0, 0, 1,    0, 0, 1,    0, 0, 1,    0, 0, 1,
            0, 1, 1,    0, 1, 1,    0, 1, 1,    0, 1, 1,
            0, 0, 0,    0, 0, 0,    0, 0, 0,    0, 0, 0,
            1, 0, 1,    1, 0, 1,    1, 0, 1,    1, 0, 1,
            1, 0, 0,    1, 0, 0,    1, 0, 0,    1, 0, 0,
            1, 1, 0,    1, 1, 0,    1, 1, 0,    1, 1, 0,
            0, 1, 0,    0, 1, 0,    0, 1, 0,    0, 1, 0,
            0, 0, 1,    0, 0, 1,    0, 0, 1,    0, 0, 1,
            0, 1, 1,    0, 1, 1,    0, 1, 1,    0, 1, 1,
            0, 0, 0,    0, 0, 0,    0, 0, 0,    0, 0, 0,
            1, 0, 1,    1, 0, 1,    1, 0, 1,    1, 0, 1,
            1, 0, 0,    1, 0, 0,    1, 0, 0,    1, 0, 0,
            1, 1, 0,    1, 1, 0,    1, 1, 0,    1, 1, 0,
            0, 1, 0,    0, 1, 0,    0, 1, 0,    0, 1, 0,
            0, 0, 1,    0, 0, 1,    0, 0, 1,    0, 0, 1,
            0, 1, 1,    0, 1, 1,    0, 1, 1,    0, 1, 1,
            0, 0, 0,    0, 0, 0,    0, 0, 0,    0, 0, 0,
            1, 0, 1,    1, 0, 1,    1, 0, 1,    1, 0, 1,
            1, 0, 0,    1, 0, 0,    1, 0, 0,    1, 0, 0,
            1, 1, 0,    1, 1, 0,    1, 1, 0,    1, 1, 0,
            0, 1, 0,    0, 1, 0,    0, 1, 0,    0, 1, 0,
            0, 0, 1,    0, 0, 1,    0, 0, 1,    0, 0, 1,
            0, 1, 1,    0, 1, 1,    0, 1, 1,    0, 1, 1,
            0, 0, 0,    0, 0, 0,    0, 0, 0,    0, 0, 0,
            1, 0, 1,    1, 0, 1,    1, 0, 1,    1, 0, 1,
            1, 0, 0,    1, 0, 0,    1, 0, 0,    1, 0, 0,
            1, 1, 0,    1, 1, 0,    1, 1, 0,    1, 1, 0,
            0, 1, 0,    0, 1, 0,    0, 1, 0,    0, 1, 0,
            0, 0, 1,    0, 0, 1,    0, 0, 1,    0, 0, 1,
            0, 1, 1,    0, 1, 1,    0, 1, 1,    0, 1, 1,
            0, 0, 0,    0, 0, 0,    0, 0, 0,    0, 0, 0,
            1, 0, 1,    1, 0, 1,    1, 0, 1,    1, 0, 1,
            1, 0, 0,    1, 0, 0,    1, 0, 0,    1, 0, 0,
            1, 1, 0,    1, 1, 0,    1, 1, 0,    1, 1, 0,
            0, 1, 0,    0, 1, 0,    0, 1, 0,    0, 1, 0,
            0, 0, 1,    0, 0, 1,    0, 0, 1,    0, 0, 1,
            0, 1, 1,    0, 1, 1,    0, 1, 1,    0, 1, 1,
            0, 0, 0,    0, 0, 0,    0, 0, 0,    0, 0, 0,
            1, 0, 1,    1, 0, 1,    1, 0, 1,    1, 0, 1,
            1, 0, 0,    1, 0, 0,    1, 0, 0,    1, 0, 0,
            1, 1, 0,    1, 1, 0,    1, 1, 0,    1, 1, 0,
            0, 1, 0,    0, 1, 0,    0, 1, 0,    0, 1, 0,
            0, 0, 1,    0, 0, 1,    0, 0, 1,    0, 0, 1,
            0, 1, 1,    0, 1, 1,    0, 1, 1,    0, 1, 1,
            0, 0, 0,    0, 0, 0,    0, 0, 0,    0, 0, 0,
            1, 0, 1,    1, 0, 1,    1, 0, 1,    1, 1, 1,
    };

   /* GLubyte indices[560];

    for (int i = 0 ; i < 560 ; i++){
        indices[i] = (GLubyte) i;
    }*/
    //Mehdi a dit sa sert a R

    glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glUseProgram(_pId);
    //glActiveTexture(GL_TEXTURE0);

    //glBindTexture(GL_TEXTURE_2D, _tId);
    glUniform1f(glGetUniformLocation(_pId,"ID" ), 0);
    glVertexAttribPointer(_posHandle, 2, GL_FLOAT, GL_FALSE, 0, position);
    glEnableVertexAttribArray(_posHandle);
    glVertexAttribPointer(_colorHandle, 3, GL_FLOAT, GL_FALSE, 0, color);
    glEnableVertexAttribArray(_colorHandle);
    //glVertexAttribPointer(_clickHandle, 2, GL_FLOAT, GL_FALSE, 0, &position);
    //glEnableVertexAttribArray(_clickHandle);
    gl4duBindMatrix("modelViewMatrix");
    gl4duLoadIdentityf();
    gl4duTranslatef(0, 0.2, -3);
    //gl4duRotated(45, 0, 0, 0);
    //gl4duRotatef(0, 0, 1, 0);
    gl4duRotatef(270.0f, 0, 0, 1);
    /* envoi de toutes les matrices stockees par GL4D */
    gl4duSendMatrices();
    for(int i = 0; i < 560; i+=4)
    {
        glDrawArrays(GL_TRIANGLE_STRIP, i, 4);
    }
   /* glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);
    glDrawArrays(GL_TRIANGLE_STRIP, 8, 4);
    glDrawArrays(GL_TRIANGLE_STRIP, 556 , 4);
 */
    /*   GLubyte ind[4];
    for (int i = 0 ; i < 140 ; i++){
        for (int j = 0 ; j < 4 ; j++){
            ind[j] = (i*4)+j;

        }
        glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, ind);
    }
*/
    //a0 += 90.0f;
}

JNIEXPORT void JNICALL Java_fr_grafeet_simplegl4d_SimpleGL4DView_ninit(JNIEnv *env, jobject obj, jstring vs_, jstring fs_) {
    const char *vs = (*env)->GetStringUTFChars(env, vs_, 0);
    const char *fs = (*env)->GetStringUTFChars(env, fs_, 0);
    init(vs, fs);
    (*env)->ReleaseStringUTFChars(env, vs_, vs);
    (*env)->ReleaseStringUTFChars(env, fs_, fs);
}

JNIEXPORT void JNICALL Java_fr_grafeet_simplegl4d_SimpleGL4DView_nreshape(JNIEnv *env, jobject obj, jint width, jint height) {
    reshape(width, height);
}

JNIEXPORT void JNICALL Java_fr_grafeet_simplegl4d_SimpleGL4DView_ndraw(JNIEnv * env, jobject obj) {
    draw();
}
