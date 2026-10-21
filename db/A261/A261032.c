/*
 * A261032  a(n) = (-1)^n*(n^8 + 4*n^7 - 14*n^5 + 28*n^3 - 17*n)/2.
 * https://oeis.org/A261032
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (-1)^n*(n^8 + 4*n^7 - 14*n^5 + 28*n^3 - 17*n)/2
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 15, i64 n <= 255,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

#ifdef __SIZEOF_INT128__
    typedef __int128 int128_t;
#else
    /* Fallback: use 64‑bit signed integer.  For seq_t of 64 bits or less,
       this suffices for all n whose value fits in seq_t. */
    typedef long long int128_t;
#endif

seq_t a(idx_t n)
{
    /* Compute each power with a wide integer type to avoid
       overflow of the intermediate results. */
    int128_t N  = (int128_t)n;
    int128_t N2 = N * N;          /* n^2 */
    int128_t N3 = N2 * N;         /* n^3 */
    int128_t N5 = N3 * N2;        /* n^5 */
    int128_t N7 = N5 * N2;        /* n^7 */
    int128_t N8 = N7 * N;         /* n^8 */

    int128_t val = N8 + 4 * N7 - 14 * N5 + 28 * N3 - 17 * N;
    val /= 2;                     /* exact division by 2 */

    if (n & 1)                     /* factor (-1)^n  */
        val = -val;

    return (seq_t)val;             /* cast to the build’s integer type */
}
