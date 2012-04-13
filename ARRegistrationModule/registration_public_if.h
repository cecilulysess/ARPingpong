#ifndef REGISTRATION_PUBLIC_IF_H_
#define REGISTRATION_PUBLIC_IF_H_


// trick to automatic import and export at both client and dll
//#ifdef LISO_CLASS_DECLSPEC
//  #undef LISO_CLASS_DECLSPEC
//#endif

#ifdef _EXPORTINGLISO
  #define LISO_CLASS_DECLSPEC    __declspec(dllexport)
#else
  #define LISO_CLASS_DECLSPEC    __declspec(dllimport)
#endif

namespace registration_module{

  // class contains series of registration operations
  class LISO_CLASS_DECLSPEC RegistrationOps {
    public:
        // Returns a + b
        static  double Add(double a, double b);
  };
} //ns registration_module

#endif //REGISTRATION_PUBLIC_IF_H_