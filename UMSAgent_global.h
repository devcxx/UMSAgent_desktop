#ifndef UMSAGENT_GLOBAL_H
#define UMSAGENT_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(UMSAGENT_LIBRARY)
#  define UMSAGENT_EXPORT Q_DECL_EXPORT
#else
#  define UMSAGENT_EXPORT Q_DECL_IMPORT
#endif

#endif // UMSAGENT_GLOBAL_H
