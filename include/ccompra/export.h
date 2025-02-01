#if !defined(CCOMPRA_EXPORT_H_LOADED)
#define CCOMPRA_EXPORT_H_LOADED

#ifdef CCOMPRA_WITH_EXECUTABLE
#define CCOMPRA_EXPORT
#else
#if defined(_WIN32) || defined(WIN32)
#ifdef CCOMPRA_LIBRARY_BUILD
#define CCOMPRA_EXPORT __declspec(dllexport)
#else
#define CCOMPRA_EXPORT __declspec(dllimport)
#endif
#else
#ifdef CCOMPRA_LIBRARY_BUILD
#define CCOMPRA_EXPORT __attribute__((visibility("default")))
#else
#define CCOMPRA_EXPORT
#endif
#endif
#endif

#if !defined(CCOMPRA_PUBLIC_FUNC)
#define CCOMPRA_PUBLIC_FUNC CCOMPRA_EXPORT
#endif // CCOMPRA_PUBLIC_FUNC

#if !defined(CCOMPRA_PRIVATE_FUNC)
#define CCOMPRA_PRIVATE_FUNC static inline
#endif // CCOMPRA_PRIVATE_FUNC

#if !defined(CCOMPRA_EXTERN_C)
#if defined(__cplusplus)
#define CCOMPRA_EXTERN_C extern "C"
#else
#define CCOMPRA_EXTERN_C
#endif // __cplusplus
#endif // CCOMPRA_EXTERN_C

#if !defined(CCOMPRA_EXTERN_C_BEG)
#if defined(__cplusplus)
#define CCOMPRA_EXTERN_C_BEG CCOMPRA_EXTERN_C {
#else
#define CCOMPRA_EXTERN_C_BEG
#endif // __cplusplus
#endif // CCOMPRA_EXTERN_C_BEG

#if !defined(CCOMPRA_EXTERN_C_END)
#if defined(__cplusplus)
#define CCOMPRA_EXTERN_C_END }
#else
#define CCOMPRA_EXTERN_C_END
#endif // __cplusplus
#endif // CCOMPRA_EXTERN_C_END

#if !defined(CCOMPRA_VER)
#define CCOMPRA_VER 1
#endif // CCOMPRA_VER

#endif // CCOMPRA_EXPORT_H_LOADED
