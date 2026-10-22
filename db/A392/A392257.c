/*
 * A392257  Smallest value of rad(t*(n-t)) where t is coprime to n and in the
 *          range 1 <= t <= n/2 where rad = A007947.
 * https://oeis.org/A392257
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = min_{1 <= t <= n/2, gcd(t,n)=1} rad(t*(n-t))
 * Range:     u8 n <= 43, u16 n <= 1703, u32 n <= 8928, u64 n <= 8928,
 *            u128 n <= 8928, big any n
 * Helpers:   rad
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the rad(idx_t) helper from ../utils/rad.h */

#include <stdbool.h>
#include "../utils/rad.h"

static inline idx_t gcd_uint64(idx_t a, idx_t b)
{
    while (b) {
        idx_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 2. For n < 2 the value is undefined; we
       simply return 0. */
    if (n < 2) return 0;

    idx_t half = n >> 1;    /* floor(n/2) */
    seq_t best = 0;
    bool first = true;

    for (idx_t t = 1; t <= half; ++t) {
        if (gcd_uint64(t, n) != 1)
            continue;

        idx_t prod = t * (n - t);   /* product fits in 64 bits for expected n */
        seq_t r = rad(prod);
        if (first || r < best) {
            best = r;
            first = false;
        }
    }
    return best;
}
