/*
 * A319932  a(n) = (1/720)*n*(n - 10)*(n - 1)*(n^3 - 34*n^2 + 181*n - 144).
 * https://oeis.org/A319932
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (1/720)*n*(n-10)*(n-1)*(n^3 - 34*n^2 + 181*n - 144)
 * Range:     i8 n <= 10, i16 n <= 29, i32 n <= 115, i64 n <= 4342,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifdef __cplusplus
#include <type_traits>

/* Integer (builtin) implementation using 128‑bit arithmetic for safety */
template<class T, std::enable_if_t<std::is_integral<T>::value, int> = 0>
static T impl(idx_t n)
{
    __int128 N = (__int128)n;
    __int128 poly = N*N*N - (__int128)34*N*N + (__int128)181*N - 144;
    __int128 tmp = N * (N-10) * (N-1) * poly;
    tmp /= 720;
    return (T)tmp;
}

/* Big‑integer (non‑integral) implementation using seq_t operators only */
template<class T, std::enable_if_t<!std::is_integral<T>::value, int> = 0>
static T impl(idx_t n)
{
    T N = (T)n;
    T poly = N*N*N - (T)34*N*N + (T)181*N - (T)144;
    T num = N * (N - (T)10) * (N - (T)1) * poly;
    num /= (T)720;
    return num;
}
#endif

seq_t a(idx_t n)
{
#ifdef __cplusplus
    return impl<seq_t>(n);
#else
    __int128 N = (__int128)n;
    __int128 poly = N*N*N - (__int128)34*N*N + (__int128)181*N - 144;
    __int128 tmp = N * (N-10) * (N-1) * poly;
    tmp /= 720;
    return (seq_t)tmp;
#endif
}
