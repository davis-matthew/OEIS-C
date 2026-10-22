/*
 * A301459  Number of 6-cycles in the n-folded cube graph.
 * https://oeis.org/A301459
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n-1) * n * (n-1) * (n-2) / 3 for n > 6 Small n values
 *            are handled explicitly
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 21, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* Handle known small cases */
    if (n < 2) return 0;
    if (n == 2 || n == 3) return 0;
    if (n == 4) return (seq_t)96;
    if (n == 5) return (seq_t)320;
    if (n == 6) return (seq_t)3200;

    /* Compute n*(n-1)*(n-2)/3 avoiding overflow */
    idx_t a1 = n;
    idx_t b1 = n - 1;
    idx_t c1 = n - 2;

    /* Divide one of the factors by 3 first */
    if (a1 % 3 == 0) {
        a1 /= 3;
    } else if (b1 % 3 == 0) {
        b1 /= 3;
    } else {
        c1 /= 3;
    }

    seq_t mult = (seq_t)a1;
    mult *= (seq_t)b1;
    mult *= (seq_t)c1;

    /* Compute 2^(n-1) using the helper seq_pow */
    seq_t pow2 = seq_pow((seq_t)2, n - 1);

    return pow2 * mult;
}
