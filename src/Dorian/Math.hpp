#ifndef DORIAN_MATH
#define DORIAN_MATH

typedef char          i8;
typedef short         i16;
typedef int           i32;
typedef long int      i64;
typedef long long int i128;

typedef unsigned char          ui8;
typedef unsigned short         ui16;
typedef unsigned int           ui32;
typedef unsigned long int      ui64;
typedef unsigned long long int ui128;

namespace Dorian::Math {
    // Different Number Classes and Structures
    template<class T>
    struct Vec2 {
        T X;
        T Y;
    };

    template<class T>
    struct Vec3 {
        T X;
        T Y;
        T Z;
    };

    // Mathematical Functions
    template<class T>
    inline Vec2<T> ScaleVec(Vec2<T> input, float scale) {
        return {(T)(input.X*scale), (T)(input.Y*scale)};
    }

    template<class T>
    inline T DotProd(Vec2<T> A, Vec2<T> B) {
        return A.X*B.X + A.Y*B.Y;
    }
}

#endif