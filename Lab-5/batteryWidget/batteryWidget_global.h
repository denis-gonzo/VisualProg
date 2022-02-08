#pragma once

#include <QtCore/qglobal.h>

#if defined(BATTERYWIDGET_LIBRARY)
#  define BATTERYWIDGET_EXPORT Q_DECL_EXPORT
#else
#  define BATTERYWIDGET_EXPORT Q_DECL_IMPORT
#endif
