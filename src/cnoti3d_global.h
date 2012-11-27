#ifndef CNOTI3DLIB_GLOBAL_H
#define CNOTI3DLIB_GLOBAL_H

#include <qglobal.h>

//#ifdef CNOTI_SHARED_LIB
//# ifdef CNOTI3DLIB_LIB
//# define CNOTI3DLIB_EXPORT Q_DECL_EXPORT
//# else
//# define CNOTI3DLIB_EXPORT Q_DECL_IMPORT
//# endif
//#else
//# define CNOTI3DLIB_EXPORT
//#endif

#if defined(CNOTI_SHARED_LIB)
#  define CNOTI3DLIB_EXPORT Q_DECL_EXPORT
#else
#  define CNOTI3DLIB_EXPORT Q_DECL_IMPORT
#endif

#endif // CNOTI3DLIB_GLOBAL_H
