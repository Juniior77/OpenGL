#include <jni.h>
#include "GL4D/gl4droid.h"

GLuint _pId, _posHandle, _colorHandle,_vTextureHandle, _tId[2] = {0};
GLfloat color[1680], width, heigh;
jboolean animError, winner;
GLfloat position[14][80];

void init(const char * vs, const char * fs) {
    glClearColor(0, 0, 0, 1);
    gl4duGenMatrix(GL_FLOAT, "modelViewMatrix");
    gl4duGenMatrix(GL_FLOAT, "projectionMatrix");
    if(!(_pId = gl4droidCreateProgram(vs, fs))) return;
    _posHandle = glGetAttribLocation(_pId, "vPosition");
    _colorHandle = glGetAttribLocation(_pId, "vColor");
    _vTextureHandle = glGetAttribLocation(_pId, "vTexture");
    for(int i = 0; i < 1680; i++)
    {
        color[i]=0.0f;
    }
    return;
}

void reshape(int w, int h) {
    width = (GLfloat)w;
    heigh = (GLfloat)h;
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

void drawWinner(jboolean _winner){
    winner = _winner;
    }

static void loadTexture(int num, int * pixels, int pw, int ph) {
    glBindTexture(GL_TEXTURE_2D, _tId[num]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, pw, ph, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
}

void draw() {

    static GLfloat a0 = 0.0f;

/*
    if(winner)
    {
        for (int i = 0; i < 14; i++) {
            for (int j = 0; j < 10; j++) {
                position[i][(j * 8) + 0] = 0;
                position[i][(j * 8) + 1] = 0;
                position[i][(j * 8) + 2] = 0;
                position[i][(j * 8) + 3] = 0;
                position[i][(j * 8) + 4] = 0;
                position[i][(j * 8) + 5] = 0;
                position[i][(j * 8) + 6] = 0;
                position[i][(j * 8) + 7] = 0;
            }
        }

        position[0][0] = -1;
        position[0][1] = -1;
        position[0][2] = 1;
        position[0][3] = -1;
        position[0][4] = -1;
        position[0][5] = 1;
        position[0][6] = 1;
        position[0][7] = 1;


        // for(int i = 0; i < 1680; i++)
        // {
        //     color[i] = 0;
        // }

        color[0] = 1;
        color[1] = 1;
        color[2] = 1;
        color[3] = 1;
        color[4] = 1;
        color[5] = 1;
        color[6] = 1;
        color[7] = 1;
        color[8] = 1;
        color[9] = 1;
        color[10] = 1;
        color[11] = 1;
    }
    else
    {*/
        GLfloat x_orig = -1;
        GLfloat y_orig = -1;
        GLfloat ratio_x = 0.2;
        GLfloat ratio_y = 0.2;
        for (int i = 0; i < 14; i++) {
            GLfloat x = x_orig + (i * ratio_x);
            for (int j = 0; j < 10; j++) {
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
    //}



    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glUseProgram(_pId);

/*    if(winner)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, _tId[1]);
        glVertexAttribPointer(_vTextureHandle, 2, GL_FLOAT, GL_FALSE, 5, (const void *)(3));
        glEnableVertexAttribArray(_vTextureHandle);
    }*/


    glUniform1f(glGetUniformLocation(_pId, "ID"), 0);
    glVertexAttribPointer(_posHandle, 2, GL_FLOAT, GL_FALSE, 0, position);
    glEnableVertexAttribArray(_posHandle);
    glVertexAttribPointer(_colorHandle, 3, GL_FLOAT, GL_FALSE, 0, color);
    glEnableVertexAttribArray(_colorHandle);

    gl4duBindMatrix("modelViewMatrix");
    gl4duLoadIdentityf();
    gl4duTranslatef(0, 0.2, -3);
    gl4duRotatef(270.0f, 0, 0, 1);

    if(animError == 1)
    {
        if(a0 < 360)
        {
            //gl4duRotatef(a0, 0, 1, 0);
            gl4duRotatef(a0, 1, 0, 0);
            a0 += 5.0f;
        }
        else{
            a0 = 0.0f;
            animError = 0;
        }

    }

    /* envoi de toutes les matrices stockees par GL4D */
    gl4duSendMatrices();
    for (int i = 0; i < 559; i += 4) {
            glDrawArrays(GL_TRIANGLE_STRIP, i, 4);
    }


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

JNIEXPORT void JNICALL Java_fr_grafeet_simplegl4d_SimpleGL4DView_ndrawWin(JNIEnv * env, jobject obj, jboolean winnerjni) {
    drawWinner(winnerjni);
}

JNIEXPORT void JNICALL Java_fr_grafeet_simplegl4d_SimpleGL4DView_nPreDraw(JNIEnv * env, jobject obj, jint indice, jfloat R, jfloat G, jfloat B) {
    majColor(indice, R, G, B);
}

JNIEXPORT void JNICALL Java_fr_grafeet_simplegl4d_SimpleGL4DView_nAnimError(JNIEnv * env, jobject obj, jboolean animateError) {
    animError = animateError;
}

JNIEXPORT void JNICALL Java_fr_grafeet_simplegl4d_SimpleGL4DView_loadTexture(JNIEnv * env, jobject obj, jint tex_number, jintArray pixels, jint pw, jint ph) {
    int * ppixels = NULL;
    if ((ppixels = (*env)->GetIntArrayElements(env, pixels, NULL)) != NULL) {
        loadTexture(tex_number, ppixels, pw, ph);
        (*env)->ReleaseIntArrayElements(env, pixels, ppixels, 0);
    } else
        loadTexture(tex_number, ppixels, 1, 1);
}