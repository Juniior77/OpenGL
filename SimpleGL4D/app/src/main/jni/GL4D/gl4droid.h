/*!\file gl4droid.h
 *
 * \brief Fonctionnalités spécifiques au système Android. 
 * \author Farès Belhadj amsi@ai.univ-paris8.fr
 * \date November 11, 2015
 */

#ifndef _GL4DROID_H
#define _GL4DROID_H

#include "gl4du.h"

#ifdef __ANDROID__

#ifdef __cplusplus
extern "C" {
#endif

/*!\brief Créé un program shader à partir des sources d'un vertex et
 *  un fragment shader passés comme chaines de caractères. 
 *
 *\param pVertexSource source du vertex shader.
 *\param pFragmentSource source du fragment shader.
 *\return l'identifiant du program shader créé.
 */
extern GLuint gl4droidCreateProgram(const char *pVertexSource, const char *pFragmentSource);

#define  GL4DROID_LOGI(...)  __android_log_print(ANDROID_LOG_INFO, __FILE__, __VA_ARGS__)
#define  GL4DROID_LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, __FILE__, __VA_ARGS__)

#ifdef __cplusplus
}
#endif

#endif

#endif
