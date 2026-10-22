/*
 * A331576  a(n) is the number of subsets of {1..n} that contain 5 even and 5
 *          odd numbers.
 * https://oeis.org/A331576
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n/2,5)^2 for n even; a(n) =
 *            binomial((n-1)/2,5)*binomial((n+1)/2,5) for n odd;
 * Range:     u8 n <= 13, u16 n <= 20, u32 n <= 51, u64 n <= 444,
 *            u128 n <= 1000, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* For n < 10, all binomials are 0, so the result is 0 automatically. */
    if ((n & 1ULL) == 0) {
        /* even n: n = 2*k  -> a = C(k,5)^2 */
        idx_t  k = n / 2;
        seq_t  b = binomial(k, 5);
        return b * b;
    } else {
        /* odd n: n = 2*k+1 -> a = C(k,5) * C(k+1,5) */
        idx_t  k  = (n - 1) / 2;   /* same as n/2 (truncated) */
        seq_t  b1 = binomial(k, 5);
        seq_t  b2 = binomial(k + 1, 5);
        return b1 * b2;
    }
}
