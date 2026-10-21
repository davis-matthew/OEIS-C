/*
 * A279783  Denominator of Farey fractions sorted according to increasing k,
 *          with k = numerator^2 + denominator^2. Fractions with same k are
 *          sorted in order of increasing denominator.
 * https://oeis.org/A279783
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   not stated
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = denominator of the nth fraction in the Farey set
   ordered by increasing k = numerator^2 + denominator^2 (Euclidean
   distance squared). For equal k the fractions are sorted by increasing
   denominator. 0/1 is the first term. */
#include <stdint.h>
#include "../utils/gcd.h"

/* Integer square root of an unsigned integer (floor). */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t x = n;
    idx_t y = (x + 1) >> 1;
    while (y < x) {
        x = y;
        y = (x + n / x) >> 1;
    }
    return x;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined below offset 1 */
    idx_t count = 0;

    for (idx_t k = 1; ; ++k) {
        /* p ranges from floor(sqrt(k/2)) down to 0 to yield
           denominators in increasing order */
        idx_t pmax = isqrt_u64(k / 2);
        for (idx_t p = pmax; ; --p) {
            idx_t rem = k - p * p;
            idx_t q   = isqrt_u64(rem);
            if (q * q == rem && q >= p && q >= 1) {
                if (gcd((seq_t)p, (seq_t)q) == 1) {
                    ++count;
                    if (count == n) return (seq_t)q;
                }
            }
            if (p == 0) break;
        }
    }
    /* unreachable */
    return 0;
}
