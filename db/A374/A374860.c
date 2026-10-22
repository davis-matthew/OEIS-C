/*
 * A374860  Obverse convolution (1)**A000201; see Comments.
 * https://oeis.org/A374860
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k=0..n} ceil(k*φ) where φ = (1+√5)/2
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>  /* for sqrtl and floorl */

/* seq_t and idx_t are defined by the build. */
seq_t a(idx_t n)
{
    static const long double phi = (1.0L + sqrtl(5.0L)) / 2.0L;
    seq_t prod = (seq_t)1;            /* a(0) = 1 */

    for (idx_t k = 1; k <= n; ++k)    /* term for k=0 is 1, already in prod */
    {
        /* ceil(k*φ) = floor(k*φ) + 1 because φ is irrational */
        uint64_t t = (uint64_t)floorl((long double)k * phi) + 1;
        prod *= (seq_t)t;
    }
    return prod;
}
