#pragma once

#ifndef STATIC_INTERSECTIONSPLITTER
    #if defined _WIN32 || defined __CYGWIN__
        #ifdef __GNUC__
          #define _API_EXPORT_DECLARATION __attribute__ ((dllexport))
          #define _API_IMPORT_DECLARATION __attribute__ ((dllimport))
        #else
          #define _API_EXPORT_DECLARATION __declspec(dllexport)
          #define _API_IMPORT_DECLARATION __declspec(dllimport)
        #endif
    #else
      #if __GNUC__ >= 4
        #define _API_EXPORT_DECLARATION __attribute__ ((visibility ("default")))
      #else
        #define _API_EXPORT_DECLARATION
      #endif
      #define _API_IMPORT_DECLARATION
    #endif
#else
    #define _API_EXPORT_DECLARATION
    #define _API_IMPORT_DECLARATION
#endif

#ifdef INTERSECTIONSPLITTER_EXPORTS
    #define INTERSECTIONSPLITTER_API _API_EXPORT_DECLARATION
#else
    #define INTERSECTIONSPLITTER_API _API_IMPORT_DECLARATION
#endif
