#ifndef __clicker_H__
#define __clicker_H__

#include <app.h>
#include <Elementary.h>
#include <system_settings.h>
#include <efl_extension.h>
#include <dlog.h>

#ifdef  LOG_TAG
#undef  LOG_TAG
#endif
#define LOG_TAG "clicker"

#if !defined(PACKAGE)
#define PACKAGE "org.example.clicker"
#endif

#define EDJ_FILE "edje/clicker.edj"
#define GRP_MAIN "main"


#endif /* __clicker_H__ */
