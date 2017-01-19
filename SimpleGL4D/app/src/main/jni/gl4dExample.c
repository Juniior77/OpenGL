#include <jni.h>
#include "GL4D/gl4droid.h"

GLuint _pId, _posHandle, _colorHandle;
GLfloat color[1680];
GLboolean GLanim;

void init(const char * vs, const char * fs) {
    glClearColor(0, 0, 0, 1);
    gl4duGenMatrix(GL_FLOAT, "modelViewMatrix");
    gl4duGenMatrix(GL_FLOAT, "projectionMatrix");
    if(!(_pId = gl4droidCreateProgram(vs, fs))) return;
    _posHandle = glGetAttribLocation(_pId, "vPosition");
    _colorHandle = glGetAttribLocation(_pId, "vColor");
    for(int i = 0; i < 1680; i++)
    {
        color[i]=0.0f;
    }
    return;
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    gl4duBindMatrix("projectionMatrix");
    gl4duLoadIdentityf();
    gl4duFrustumf(-0.5, 0.5, -0.5 * h / w, 0.5 * h / w, 1.5, 1000.0);
    gl4duBindMatrix("modelViewMatrix");
}

void majColor(int indice, GLfloat R, GLfloat G, GLfloat B){
    color[indice] = (GLfloat) R;
    color[indice+1] = (GLfloat) G;
    color[indice+2] = (GLfloat) B;

    color[indice+3] = (GLfloat) R;
    color[indice+4] = (GLfloat) G;
    color[indice+5] = (GLfloat) B;

    color[indice+6] = (GLfloat) R;
    color[indice+7] = (GLfloat) G;
    color[indice+8] = (GLfloat) B;

    color[indice+9] = (GLfloat) R;
    color[indice+10] = (GLfloat) G;
    color[indice+11] = (GLfloat) B;
}

void draw(void){
    static GLfloat a0 = 0.0f;

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

    glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glUseProgram(_pId);

    glUniform1f(glGetUniformLocation(_pId,"ID" ), 0);
    glVertexAttribPointer(_posHandle, 2, GL_FLOAT, GL_FALSE, 0, position);
    glEnableVertexAttribArray(_posHandle);
    glVertexAttribPointer(_colorHandle, 3, GL_FLOAT, GL_FALSE, 0, color);
    glEnableVertexAttribArray(_colorHandle);
    gl4duBindMatrix("modelViewMatrix");
    gl4duLoadIdentityf();
    gl4duTranslatef(0, 0.2, -3);
    gl4duRotatef(270.0f, 0, 0, 1);
    //gl4duRotatef(a0, 0, 1, 0);
    gl4duRotatef(2 * a0, 1, 0, 0);

    /* envoi de toutes les matrices stockees par GL4D */
    gl4duSendMatrices();
    for(int i = 0; i < 560; i+=4)
    {
        glDrawArrays(GL_TRIANGLE_STRIP, i, 4);
    }
    a0 += 1.4f;
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

JNIEXPORT void JNICALL Java_fr_grafeet_simplegl4d_SimpleGL4DView_nPreDraw(JNIEnv * env, jobject obj, jint indice, jfloat R, jfloat G, jfloat B) {
    majColor(indice, R, G, B);
}
JNIEXPORT void JNICALL Java_fr_grafeet_simplegl4d_SimpleGL4DView_nAnim(JNIEnv * env, jobject obj, jboolean anim) {
    GLanim = (GLboolean)anim;
}