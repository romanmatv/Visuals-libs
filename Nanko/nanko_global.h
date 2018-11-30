#ifndef SPEKTRO_GLOBAL_H
#define SPEKTRO_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(SPEKTROCOLOR_LIBRARY)
#  define NANKOSHARED_EXPORT Q_DECL_EXPORT
#else
#  define NANKOSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // SPEKTRO_GLOBAL_H
