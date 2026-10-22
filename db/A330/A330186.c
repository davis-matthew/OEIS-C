/*
 * A330186  a(n) = n + floor(n*r/t) + floor(n*s/t), where r = phi - 1/2, s =
 *          phi, t = phi + 1/2, phi = golden ratio = (1+sqrt(5))/2.
 * https://oeis.org/A330186
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + floor(n*(5-2*sqrt(5))) + floor(n*(3-sqrt(5)))
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* 5 - 2*sqrt(5)  = (51841 - 2*23184*sqrt(5))/23184  (rational approximation)
   3 - sqrt(5)     = (68827 - 23184*sqrt(5))/23184   (not used directly)   */

/* 51841/23184 is a convergent of sqrt(5) with error < 1/23184²,
   so for all n <= 10000 we have |n*51841/23184 - n*sqrt(5)| < 1.
   Consequently floor(n*sqrt(5)) = (n*51841)/23184 exactly.    */

seq_t a(idx_t n)
{
    /* 51841/23184 ≈ 2.2360679775 (slightly greater than sqrt(5)) */
    const uint64_t P = 51841;
    const uint64_t Q = 23184;

    seq_t N = (seq_t)n;

    /* floor(n*sqrt(5)) */
    seq_t fsqrt5 = (seq_t)(((__uint128_t)N * P) / Q);

    /* floor(2*n*sqrt(5)) */
    seq_t f2sqrt5 = (seq_t)(((__uint128_t)(N + N) * P) / Q);

    /* a(n) = 9*n - floor(n*sqrt(5)) - floor(2*n*sqrt(5)) - 2 */
    return 9 * N - fsqrt5 - f2sqrt5 - 2;
}
