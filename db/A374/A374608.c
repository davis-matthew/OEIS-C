/*
 * A374608  a(n) is the denominator of (1134*n^3 + 2097*n^2 + 1188*n +
 *          193)/(10368*n^4 + 20736*n^3 + 14112*n^2 + 3744*n + 320).
 * https://oeis.org/A374608
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of (1134*n^3 + 2097*n^2 + 1188*n + 193) /*
 *            -------------------------------------------- /* (10368*n^4 +
 *            20736*n^3 + 14112*n^2 + 3744*n + 320) /* Reduced by gcd of
 *            numerator and denominator. See A374608 for /* definition
 * Range:     u8 none, u16 n <= 1, u32 n <= 25, u64 n <= 6495, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
 
/* Helper: Euclidean algorithm for __int128 values.  */
static inline __int128 gcd128(__int128 a, __int128 b) {
    while (b) {
        __int128 t = b;
        b = a % b;
        a = t;
    }
    return a;
}
 
seq_t a(idx_t n) {
    /* Promote index to a 128‑bit integer for safe intermediate
     * arithmetic.  All coefficients are small, so the 128‑bit
     * space is more than enough for n up to 100 000 and beyond.  */
    __int128 N = (__int128)n;
 
    /* Powers of n needed for the polynomials.  */
    __int128 n2 = N * N;        /* n^2 */
    __int128 n3 = n2 * N;       /* n^3 */
    __int128 n4 = n2 * n2;      /* n^4 */
 
    /* Numerator: 1134*n^3 + 2097*n^2 + 1188*n + 193 */
    __int128 num = (__int128)1134 * n3
                   + (__int128)2097 * n2
                   + (__int128)1188 * N
                   + 193;
 
    /* Denominator: 10368*n^4 + 20736*n^3 + 14112*n^2 + 3744*n + 320 */
    __int128 den = (__int128)10368 * n4
                   + (__int128)20736 * n3
                   + (__int128)14112 * n2
                   + (__int128)3744 * N
                   + 320;
 
    __int128 g = gcd128(num, den);
    return (seq_t)(den / g);
}
