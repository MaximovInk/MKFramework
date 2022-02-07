
#define MK_ENGINE_EXPORTS

#ifdef MK_ENGINE_EXPORTS

#define MK_ENGINE_API __declspec(dllexport)
#else
#define MK_ENGINE_API __declspec(dllimport)
#endif