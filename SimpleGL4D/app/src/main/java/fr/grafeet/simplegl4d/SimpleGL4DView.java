/*
 * Copyright (C) 2009 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package fr.grafeet.simplegl4d;
/*
 * Copyright (C) 2008 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


import android.app.Activity;
import android.content.Context;
import android.graphics.PixelFormat;
import android.opengl.GLES32;
import android.opengl.GLSurfaceView;
import android.util.Log;
import android.view.MotionEvent;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.Random;
import java.util.Timer;
import java.util.TimerTask;
import java.util.concurrent.ThreadFactory;

import javax.microedition.khronos.egl.EGL10;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.egl.EGLContext;
import javax.microedition.khronos.egl.EGLDisplay;
import javax.microedition.khronos.opengles.GL10;

/**
 * A simple GLSurfaceView sub-class that demonstrate how to perform
 * OpenGL ES 2.0 rendering into a GL Surface. Note the following important
 * details:
 *
 * - The class must use a custom context factory to enable 2.0 rendering.
 *   See ContextFactory class definition below.
 *
 * - The class must use a custom EGLConfigChooser to be able to select
 *   an EGLConfig that supports 2.0. This is done by providing a config
 *   specification to eglChooseConfig() that has the attribute
 *   EGL10.ELG_RENDERABLE_TYPE containing the EGL_OPENGL_ES2_BIT flag
 *   set. See ConfigChooser class definition below.
 *
 * - The class must select the surface's format, then choose an EGLConfig
 *   that matches it exactly (with regards to red/green/blue/alpha channels
 *   bit depths). Failure to do so would result in an EGL_BAD_MATCH error.
 */

/**
 * Modifyed by Farès Belhadj amsi@ai.univ-paris8.fr on November 4, 2016
 * Adding :
 * - support to external shader programs
 * - Modifying Renderer process to avoid launching bug on some devices
 */

class SimpleGL4DView extends GLSurfaceView {
    static {
        System.loadLibrary("gl4dExample");
    }
    public static native void ninit(String vs, String fs);
    public static native void ndraw();
    public static native void nreshape(int w, int h);
    public static native void nPreDraw(int indice, float R, float G, float B);
    public static native void nAnim(boolean animate);

    private int carteTileSize, posClickX, posClickY, carteTop, carteLeft, score;
    private int posCouleurGauche, posCouleurDroite, posCouleurHaut, posCouleurBas;
    private static int carteWidth = 10, carteHeight = 14;
    private boolean animation;

    public ArrayList tabCol = new ArrayList();
    int[][] carte = new int[14][10];
    private Timer timer;
    private TimerTask timerTask;

    private static String TAG = "Camera2GLView";
    private static String _vshader = null;
    private static String _fshader = null;
    private static final boolean DEBUG = true;
    Renderer _renderer = new Renderer();
    int _cameraWidth = 320, _cameraHeight = 200;
    public SimpleGL4DView(Context context) {
        super(context);
        init(false, 0, 0);
    }

    private void init(boolean translucent, int depth, int stencil) {
        if(_vshader == null)
            _vshader = readRawTextFile(getContext(), R.raw.basic_vs);
        if(_fshader == null)
            _fshader = readRawTextFile(getContext(), R.raw.basic_fs);
        /* By default, GLSurfaceView() creates a RGB_565 opaque surface.
         * If we want a translucent one, we should change the surface's
         * format here, using PixelFormat.TRANSLUCENT for GL Surfaces
         * is interpreted as any 32-bit surface with alpha by SurfaceFlinger.
         */
        if (translucent) {
            this.getHolder().setFormat(PixelFormat.TRANSLUCENT);
        }

        /* Setup the context factory for 2.0 rendering.
         * See ContextFactory class definition below
         */
        setEGLContextFactory(new ContextFactory());

        /* We need to choose an EGLConfig that matches the format of
         * our surface exactly. This is going to be done in our
         * custom config chooser. See ConfigChooser class definition
         * below.
         */
        setEGLConfigChooser( translucent ?
                             new ConfigChooser(8, 8, 8, 8, depth, stencil) :
                             new ConfigChooser(5, 6, 5, 0, depth, stencil) );

        /* Set the renderer responsible for frame rendering */
        setRenderer(_renderer);
    }
    static String readRawTextFile(Context ctx, int resId) {
        InputStream inputStream = ctx.getResources().openRawResource(resId);
        InputStreamReader inputreader = new InputStreamReader(inputStream);
        BufferedReader buffreader = new BufferedReader(inputreader);
        String line;
        StringBuilder text = new StringBuilder();
        try {
            while (( line = buffreader.readLine()) != null) {
                text.append(line);
                text.append('\n');
            }
        } catch (IOException e) {
            return null;
        }
        return text.toString();
    }

    private static class ContextFactory implements EGLContextFactory {
        private static int EGL_CONTEXT_CLIENT_VERSION = 0x3098;
        public EGLContext createContext(EGL10 egl, EGLDisplay display, EGLConfig eglConfig) {
            Log.w(TAG, "creating OpenGL ES 2.0 context");
            checkEglError("Before eglCreateContext", egl);
            int[] attrib_list = {EGL_CONTEXT_CLIENT_VERSION, 2, EGL10.EGL_NONE };
            EGLContext context = egl.eglCreateContext(display, eglConfig, EGL10.EGL_NO_CONTEXT, attrib_list);
            checkEglError("After eglCreateContext", egl);
            return context;
        }

        public void destroyContext(EGL10 egl, EGLDisplay display, EGLContext context) {
            egl.eglDestroyContext(display, context);
        }
    }

    private static void checkEglError(String prompt, EGL10 egl) {
        int error;
        while ((error = egl.eglGetError()) != EGL10.EGL_SUCCESS) {
            Log.e(TAG, String.format("%s: EGL error: 0x%x", prompt, error));
        }
    }

    private static class ConfigChooser implements EGLConfigChooser {

        public ConfigChooser(int r, int g, int b, int a, int depth, int stencil) {
            mRedSize = r;
            mGreenSize = g;
            mBlueSize = b;
            mAlphaSize = a;
            mDepthSize = depth;
            mStencilSize = stencil;
        }

        /* This EGL config specification is used to specify 2.0 rendering.
         * We use a minimum size of 4 bits for red/green/blue, but will
         * perform actual matching in chooseConfig() below.
         */
        private static int EGL_OPENGL_ES2_BIT = 4;
        private static int[] s_configAttribs2 =
        {
            EGL10.EGL_RED_SIZE, 4,
            EGL10.EGL_GREEN_SIZE, 4,
            EGL10.EGL_BLUE_SIZE, 4,
            EGL10.EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
            EGL10.EGL_NONE
        };

        public EGLConfig chooseConfig(EGL10 egl, EGLDisplay display) {

            /* Get the number of minimally matching EGL configurations
             */
            int[] num_config = new int[1];
            egl.eglChooseConfig(display, s_configAttribs2, null, 0, num_config);

            int numConfigs = num_config[0];

            if (numConfigs <= 0) {
                throw new IllegalArgumentException("No configs match configSpec");
            }

            /* Allocate then read the array of minimally matching EGL configs
             */
            EGLConfig[] configs = new EGLConfig[numConfigs];
            egl.eglChooseConfig(display, s_configAttribs2, configs, numConfigs, num_config);

            if (DEBUG) {
                 printConfigs(egl, display, configs);
            }
            /* Now return the "best" one
             */
            return chooseConfig(egl, display, configs);
        }

        public EGLConfig chooseConfig(EGL10 egl, EGLDisplay display,
                EGLConfig[] configs) {
            for(EGLConfig config : configs) {
                int d = findConfigAttrib(egl, display, config,
                        EGL10.EGL_DEPTH_SIZE, 0);
                int s = findConfigAttrib(egl, display, config,
                        EGL10.EGL_STENCIL_SIZE, 0);

                // We need at least mDepthSize and mStencilSize bits
                if (d < mDepthSize || s < mStencilSize)
                    continue;

                // We want an *exact* match for red/green/blue/alpha
                int r = findConfigAttrib(egl, display, config,
                        EGL10.EGL_RED_SIZE, 0);
                int g = findConfigAttrib(egl, display, config,
                            EGL10.EGL_GREEN_SIZE, 0);
                int b = findConfigAttrib(egl, display, config,
                            EGL10.EGL_BLUE_SIZE, 0);
                int a = findConfigAttrib(egl, display, config,
                        EGL10.EGL_ALPHA_SIZE, 0);

                if (r == mRedSize && g == mGreenSize && b == mBlueSize && a == mAlphaSize)
                    return config;
            }
            return null;
        }

        private int findConfigAttrib(EGL10 egl, EGLDisplay display,
                EGLConfig config, int attribute, int defaultValue) {

            if (egl.eglGetConfigAttrib(display, config, attribute, mValue)) {
                return mValue[0];
            }
            return defaultValue;
        }

        private void printConfigs(EGL10 egl, EGLDisplay display,
            EGLConfig[] configs) {
            int numConfigs = configs.length;
            Log.w(TAG, String.format("%d configurations", numConfigs));
            for (int i = 0; i < numConfigs; i++) {
                Log.w(TAG, String.format("Configuration %d:\n", i));
                printConfig(egl, display, configs[i]);
            }
        }

        private void printConfig(EGL10 egl, EGLDisplay display,
                EGLConfig config) {
            int[] attributes = {
                    EGL10.EGL_BUFFER_SIZE,
                    EGL10.EGL_ALPHA_SIZE,
                    EGL10.EGL_BLUE_SIZE,
                    EGL10.EGL_GREEN_SIZE,
                    EGL10.EGL_RED_SIZE,
                    EGL10.EGL_DEPTH_SIZE,
                    EGL10.EGL_STENCIL_SIZE,
                    EGL10.EGL_CONFIG_CAVEAT,
                    EGL10.EGL_CONFIG_ID,
                    EGL10.EGL_LEVEL,
                    EGL10.EGL_MAX_PBUFFER_HEIGHT,
                    EGL10.EGL_MAX_PBUFFER_PIXELS,
                    EGL10.EGL_MAX_PBUFFER_WIDTH,
                    EGL10.EGL_NATIVE_RENDERABLE,
                    EGL10.EGL_NATIVE_VISUAL_ID,
                    EGL10.EGL_NATIVE_VISUAL_TYPE,
                    0x3030, // EGL10.EGL_PRESERVED_RESOURCES,
                    EGL10.EGL_SAMPLES,
                    EGL10.EGL_SAMPLE_BUFFERS,
                    EGL10.EGL_SURFACE_TYPE,
                    EGL10.EGL_TRANSPARENT_TYPE,
                    EGL10.EGL_TRANSPARENT_RED_VALUE,
                    EGL10.EGL_TRANSPARENT_GREEN_VALUE,
                    EGL10.EGL_TRANSPARENT_BLUE_VALUE,
                    0x3039, // EGL10.EGL_BIND_TO_TEXTURE_RGB,
                    0x303A, // EGL10.EGL_BIND_TO_TEXTURE_RGBA,
                    0x303B, // EGL10.EGL_MIN_SWAP_INTERVAL,
                    0x303C, // EGL10.EGL_MAX_SWAP_INTERVAL,
                    EGL10.EGL_LUMINANCE_SIZE,
                    EGL10.EGL_ALPHA_MASK_SIZE,
                    EGL10.EGL_COLOR_BUFFER_TYPE,
                    EGL10.EGL_RENDERABLE_TYPE,
                    0x3042 // EGL10.EGL_CONFORMANT
            };
            String[] names = {
                    "EGL_BUFFER_SIZE",
                    "EGL_ALPHA_SIZE",
                    "EGL_BLUE_SIZE",
                    "EGL_GREEN_SIZE",
                    "EGL_RED_SIZE",
                    "EGL_DEPTH_SIZE",
                    "EGL_STENCIL_SIZE",
                    "EGL_CONFIG_CAVEAT",
                    "EGL_CONFIG_ID",
                    "EGL_LEVEL",
                    "EGL_MAX_PBUFFER_HEIGHT",
                    "EGL_MAX_PBUFFER_PIXELS",
                    "EGL_MAX_PBUFFER_WIDTH",
                    "EGL_NATIVE_RENDERABLE",
                    "EGL_NATIVE_VISUAL_ID",
                    "EGL_NATIVE_VISUAL_TYPE",
                    "EGL_PRESERVED_RESOURCES",
                    "EGL_SAMPLES",
                    "EGL_SAMPLE_BUFFERS",
                    "EGL_SURFACE_TYPE",
                    "EGL_TRANSPARENT_TYPE",
                    "EGL_TRANSPARENT_RED_VALUE",
                    "EGL_TRANSPARENT_GREEN_VALUE",
                    "EGL_TRANSPARENT_BLUE_VALUE",
                    "EGL_BIND_TO_TEXTURE_RGB",
                    "EGL_BIND_TO_TEXTURE_RGBA",
                    "EGL_MIN_SWAP_INTERVAL",
                    "EGL_MAX_SWAP_INTERVAL",
                    "EGL_LUMINANCE_SIZE",
                    "EGL_ALPHA_MASK_SIZE",
                    "EGL_COLOR_BUFFER_TYPE",
                    "EGL_RENDERABLE_TYPE",
                    "EGL_CONFORMANT"
            };
            int[] value = new int[1];
            for (int i = 0; i < attributes.length; i++) {
                int attribute = attributes[i];
                String name = names[i];
                if ( egl.eglGetConfigAttrib(display, config, attribute, value)) {
                    Log.w(TAG, String.format("  %s: %d\n", name, value[0]));
                } else {
                    // Log.w(TAG, String.format("  %s: failed\n", name));
                    while (egl.eglGetError() != EGL10.EGL_SUCCESS);
                }
            }
        }

        // Subclasses can adjust these values:
        protected int mRedSize;
        protected int mGreenSize;
        protected int mBlueSize;
        protected int mAlphaSize;
        protected int mDepthSize;
        protected int mStencilSize;
        private int[] mValue = new int[1];
    }

    @Override
    protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
        super.onMeasure(widthMeasureSpec, heightMeasureSpec);
        _cameraWidth = MeasureSpec.getSize(widthMeasureSpec);
        _cameraHeight = MeasureSpec.getSize(heightMeasureSpec);
        System.out.println("W = " + _cameraWidth + ", H = " + _cameraHeight);
    }

    private interface Idraw {
        void draw();
    }
    private class Renderer implements GLSurfaceView.Renderer {
        private boolean _hasInit = false;
        private Idraw _idInit = new Idraw() {
            @Override
            public void draw() {
                ninit(_vshader, _fshader);
                _hasInit = true;
                _idInitOrDraw = _idDraw;
                _idInitOrDraw.draw();
                loadRandCol(8);
            }
        };
        private Idraw _idDraw = new Idraw() {
            @Override
            public void draw() {
                ndraw();
            }
        };
        private Idraw _idInitOrDraw = _idInit;

        public void onDrawFrame(GL10 gl) {
            _idInitOrDraw.draw();
        }
        public void onSurfaceChanged(GL10 gl, int width, int height) {
            if(!_hasInit)
                _idInitOrDraw.draw();
            nreshape(width, height);
        }
        public void onSurfaceCreated(GL10 gl, EGLConfig config) { }
    }
    @Override
    public boolean onTouchEvent(MotionEvent event) {

        carteTileSize = getWidth() / carteWidth;
        carteTop = ((getHeight() - carteHeight * carteTileSize) / 2)+carteTileSize;
        carteLeft = ((getWidth() - carteWidth * carteTileSize) / 2);

        posClickX = (int)(event.getX() - carteLeft) / carteTileSize;
        posClickY = (int)(event.getY() - carteTop) / carteTileSize;
        int monIndice = (posClickY * 10) + posClickX;

        //Log.i("->FCT<-", "X: " + posClickX + " Y: " + posClickY);
        CheckCarteMatch(posClickX, posClickY);
        return true;
    }

    public void majColorGL(int indice)
    {
        //Log.i("->FCT<-", "Indice: " + indice);
        nPreDraw(indice*12, 0.1f, 0.1f, 0.1f);
    }
    //*********************************************************
    //                      Gestion du jeux
    //*********************************************************

    private void loadRandCol(int leNbCouleur) {
        int nbCouleur = leNbCouleur, nbColMax = 120;
        int nbMax = (nbColMax / nbCouleur);
        int ecart = (nbMax / 3);
        int nbColRandMax = nbMax + ecart;
        int nbColMin = nbMax - ecart;

        Random rand = new Random();

        tabCol.add(0, 20);
        //Log.i("-> FCT <-", "tabCol0: NbCol: " + tabCol.get(0));
        for (int i = 1; i < nbCouleur; i++) {
            int colRand = nbColMin + rand.nextInt(nbColRandMax - nbColMin);
            if ((colRand % 2) != 0) {
                colRand += 1;
            }
            tabCol.add(i, colRand);
            nbColMax -= colRand;
            //Log.i("-> FCT <-", "tabCol[i]: " + i + " nbColMax: " + tabCol.get(i));
        }
        tabCol.add(nbCouleur, nbColMax);
        //Log.i("-> FCT <-", "tabCol8: nbColMax: " + tabCol.get(8));
        loadCarte();
    }

    //Chargement de la carte
    private void loadCarte() {
        Random rand = new Random();
        animation = true;
        nAnim(animation);
        for (int i = 0; i < carteHeight; i++) {
            for (int j = 0; j < carteWidth; j++) {
                int myCol = 0 + rand.nextInt(tabCol.size() - 0);
                if ((int) tabCol.get(myCol) == 0) {
                    while ((int) tabCol.get(myCol) == 0) {
                        myCol = 0 + rand.nextInt(tabCol.size() - 0);
                    }
                }
                carte[i][j] = myCol;
                tabCol.set(myCol, ((int) tabCol.get(myCol)) - 1);
                loadGlColor(myCol, i, j);
                
                //Log.i("-> FCT <-", "tabCol[" + i + "][" + j + "] :" + " rand: " + myCol + " Reste de tabCol[" + myCol + "]: " + tabCol.get(myCol));
            }
        }
        animation = false;
        nAnim(animation);
    }

    private void loadGlColor(int couleur, int height, int width){
        int i = (height * 10) + width;
switch (couleur){
        case 0:
        nPreDraw(i*12, 0.1f, 0.1f, 0.1f);
        break;
        case 1:
        nPreDraw(i*12, 1, 0, 0);
        break;
        case 2:
        nPreDraw(i*12, 0, 1, 0);
        break;
        case 3:
        nPreDraw(i*12, 0, 0, 1);
        break;
        case 4:
        nPreDraw(i*12, 1, 1, 0);
        break;
        case 5:
        nPreDraw(i*12, 0, 1, 1);
        break;
        case 6:
        nPreDraw(i*12, 1, 0, 1);
        break;
        case 7:
        nPreDraw(i*12, 0.5f, 1, 0);
        break;
        case 8:
        nPreDraw(i*12, 0, 0.5f, 1);
        break;
        }
    }


    //Gestion du match des couleurs
    private void CheckCarteMatch(int x, int y)
    {
        if(carte[y][x] == 0) {
            int couleurGauche = getGauche(x, y);
            int couleurDroite = getDroite(x, y);
            int couleurHaut = getHaut(x, y);
            int couleurBas = getBas(x, y);
            int scoreTmp = score; //Permet de verifier si le score a deja été MAJ

            //Log.i("-> FCT <-", "Couleur de droite  : " + couleurDroite);
            //Log.i("-> FCT <-", "Couleur de gauche  : " + couleurGauche);
            //Log.i("-> FCT <-", "Couleur du haut    : " + couleurHaut);
            //Log.i("-> FCT <-", "Couleur du bas     : " + couleurBas);

            //Gestion des Quadruples
            if(couleurGauche == couleurDroite && couleurHaut == couleurBas && couleurGauche != 0 && couleurHaut != 0) {
                carte[y][posCouleurGauche] = 0;
                majColorGL((y*10)+posCouleurGauche);
                carte[y][posCouleurDroite] = 0;
                majColorGL((y*10)+posCouleurDroite);
                carte[posCouleurHaut][x] = 0;
                majColorGL((posCouleurHaut*10)+x);
                carte[posCouleurBas][x] = 0;
                majColorGL((posCouleurBas*10)+x);
                score += 120;
            }
            else if(score == scoreTmp){
                //Gestion des Triples
                if (couleurDroite == couleurHaut && couleurDroite == couleurBas && couleurDroite != 0) {
                    //clickSound();
                    carte[y][posCouleurDroite] = 0;
                    majColorGL((y*10)+posCouleurDroite);
                    carte[posCouleurHaut][x] = 0;
                    majColorGL((posCouleurHaut*10)+x);
                    carte[posCouleurBas][x] = 0;
                    majColorGL((posCouleurBas*10)+x);
                    score += 60;
                }
                if (couleurHaut == couleurDroite && couleurHaut == couleurGauche && couleurHaut != 0) {
                    //clickSound();
                    carte[posCouleurHaut][x] = 0;
                    majColorGL((posCouleurHaut*10)+x);
                    carte[y][posCouleurDroite] = 0;
                    majColorGL((y*10)+posCouleurDroite);
                    carte[y][posCouleurGauche] = 0;
                    majColorGL((y*10)+posCouleurGauche);
                    score += 60;
                }
                if (couleurGauche == couleurHaut && couleurGauche == couleurBas && couleurGauche != 0) {
                    //clickSound();
                    carte[y][posCouleurGauche] = 0;
                    majColorGL((y*10)+posCouleurGauche);
                    carte[posCouleurHaut][x] = 0;
                    majColorGL((posCouleurHaut*10)+x);
                    carte[posCouleurBas][x] = 0;
                    majColorGL((posCouleurBas*10)+x);
                    score += 60;
                }
                if (couleurBas == couleurDroite && couleurBas == couleurGauche && couleurBas != 0) {
                    //clickSound();
                    carte[posCouleurBas][x] = 0;
                    majColorGL((posCouleurBas*10)+x);
                    carte[y][posCouleurDroite] = 0;
                    majColorGL((y*10)+posCouleurDroite);
                    carte[y][posCouleurGauche] = 0;
                    majColorGL((y*10)+posCouleurGauche);
                    score += 60;
                }
                else if (score == scoreTmp){

                    //Gestion des Double
                    if (couleurDroite == couleurHaut && couleurDroite != 0) {
                        //clickSound();
                        carte[y][posCouleurDroite] = 0;
                        majColorGL((y*10)+posCouleurDroite);
                        carte[posCouleurHaut][x] = 0;
                        majColorGL((posCouleurHaut*10)+x);

                        score += 40;
                    }
                    if (couleurHaut == couleurGauche && couleurHaut != 0) {
                        //clickSound();
                        carte[posCouleurHaut][x] = 0;
                        majColorGL((posCouleurHaut*10)+x);
                        carte[y][posCouleurGauche] = 0;
                        majColorGL((y*10)+posCouleurGauche);
                        score += 40;
                    }
                    if (couleurGauche == couleurBas && couleurGauche != 0) {
                        //clickSound();
                        carte[y][posCouleurGauche] = 0;
                        majColorGL((y*10)+posCouleurGauche);
                        carte[posCouleurBas][x] = 0;
                        majColorGL((posCouleurBas*10)+x);
                        score += 40;
                    }
                    if (couleurBas == couleurDroite && couleurBas != 0) {
                        //clickSound();
                        carte[posCouleurBas][x] = 0;
                        majColorGL((posCouleurBas*10)+x);
                        carte[y][posCouleurDroite] = 0;
                        majColorGL((y*10)+posCouleurDroite);
                        score += 40;
                    }
                    if (couleurBas == couleurHaut && couleurBas != 0) {
                        //clickSound();
                        carte[posCouleurBas][x] = 0;
                        majColorGL((posCouleurBas*10)+x);
                        carte[posCouleurHaut][x] = 0;
                        majColorGL((posCouleurHaut*10)+x);
                        score += 40;
                    }
                    if (couleurDroite == couleurGauche && couleurDroite != 0) {
                        //clickSound();
                        carte[y][posCouleurDroite] = 0;
                        majColorGL((y*10)+posCouleurDroite);
                        carte[y][posCouleurGauche] = 0;
                        majColorGL((y*10)+posCouleurGauche);
                        score += 40;
                    }
                    else if(score == scoreTmp)
                    {
                        //Enlève du temps;
                    }
                }
            }
        }
    }


    //Stock la couleur de gauche
    private int getGauche (int x, int y)
    {
        int colorNegFirst = 0;
        for(int i = x; i >= 0; i-- )
        {
            if(carte[y][i]!=0)
            {
                colorNegFirst = carte[y][i];
                posCouleurGauche = i;
                break;
            }
        }
        return colorNegFirst;
    }

    //Stock la couleur de droite
    private int getDroite(int x, int y)
    {
        int colorPosFirst = 0;
        for(int i = x; i <= 9; i++ )
        {
            if(carte[y][i]!=0)
            {
                colorPosFirst = carte[y][i];
                posCouleurDroite = i;
                break;
            }
            //Log.i("-> FCT <-", "Incrémentation de i: " + i);
        }
        return colorPosFirst;
    }

    //Stock la couleur du haut
    private int getHaut(int x, int y)
    {
        int colorNegFirst = 0;
        for(int i = y; i >= 0; i-- )
        {
            if(carte[i][x]!=0)
            {
                colorNegFirst = carte[i][x];
                posCouleurHaut = i;
                break;
            }
        }
        return colorNegFirst;
    }

    //Stock la couleur du bas
    private int getBas(int x, int y)
    {
        int colorPosFirst = 0;
        for(int i = y; i <= 13; i++ )
        {
            if(carte[i][x]!=0)
            {
                colorPosFirst = carte[i][x];
                posCouleurBas = i;
                break;
            }
            //Log.i("-> FCT <-", "Incrémentation de i: " + i);
        }
        return colorPosFirst;
    }
}