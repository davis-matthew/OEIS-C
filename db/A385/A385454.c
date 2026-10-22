/*
 * A385454  Difference of the largest and smallest semiperimeters of an
 *          integral rectangle with area n.
 * https://oeis.org/A385454
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + n - A063655(n) (minimal semiperimeter of an integral
 *            rectangle with area n)
 * Range:     u8 n <= 287, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* integer square root: floor(sqrt(n)) for 64‑bit unsigned n */
static idx_t
floor_sqrt_u64(idx_t n)
{
    if (n == 0 || n == 1) return n;
    idx_t x   = 0;
    idx_t bit = ((idx_t)1) << 62;            /* largest power of four ≤ n */

    while (bit > n) bit >>= 2;              /* shift to find starting bit */

    while (bit != 0) {
        if (n >= x + bit) {
            n -= x + bit;
            x = (x >> 1) + bit;
        } else {
            x >>= 1;
        }
        bit >>= 2;
    }
    return x;
}

/* OEIS A385454: difference of largest and smallest semiperimeters */
seq_t
a(idx_t n)
{
    /* The sequence is defined for n ≥ 1, but handling n==0 does not hurt. */
    if (n == 0) return (seq_t)0;

    /* Find the minimal semiperimeter A063655(n). */
    idx_t root    = floor_sqrt_u64(n);
    seq_t min_sum = (seq_t)n + (seq_t)1;    /* initialise with the largest possible sum (1 + n) */

    for (idx_t d = 1; d <= root; ++d) {
        if (n % d == 0) {
            idx_t e   = n / d;
            seq_t sum = (seq_t)d + (seq_t)e;   /* semiperimeter of the rectangle d × e */
            if (sum < min_sum) min_sum = sum;
        }
    }

    /* a(n) = 1 + n - minimal_semiperimeter */
    return (seq_t)1 + (seq_t)n - min_sum;
}
