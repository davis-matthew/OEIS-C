/*
 * A359730  a(n) = Sum_{d|n} 2^(d-1) * d^(n/d).
 * https://oeis.org/A359730
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} 2^(d-1) * d^(n/d)
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 28, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   big_pow, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/big_pow.h"

/* Computes the n-th term of OEIS sequence A359730.
   n must be >= 1. */
seq_t a(idx_t n)
{
    if (n == 0)   /* offset safety; not used for valid n */
        return 0;

    seq_t sum = 0;

    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d != 0)
            continue;

        idx_t other = n / d;

        /* term for divisor d */
        seq_t term1 = pow2(d - 1) * big_pow((seq_t)d, (seq_t)other);
        sum += term1;

        /* if d and other are distinct, add term for other */
        if (other != d) {
            seq_t term2 = pow2(other - 1) * big_pow((seq_t)other, (seq_t)d);
            sum += term2;
        }
    }

    return sum;
}
