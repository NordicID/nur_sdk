#ifndef _NURAPI_EXPORT_H_
#define _NURAPI_EXPORT_H_

#if !defined(NUR_STATICLIB) && !defined(NUR_LINUX)
#ifdef NUR_EXPORTS
# define NUR_API __declspec(dllexport)
#else
# define NUR_API __declspec(dllimport)
#endif
#else
# define NUR_API 
#endif

#define NURAPICALLBACK	__stdcall
#define NURAPICONV		__stdcall

#endif
