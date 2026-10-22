/*
 * A339358  Maximum number of copies of a 1234567 permutation pattern in an
 *          alternating (or zig-zag) permutation of length n + 11.
 * https://oeis.org/A339358
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 128*C(n/2+6,7) - 64*C(n/2+5,6) if n is even a(n) =
 *            128*C((n+1)/2+4,7) + 128*C((n+1)/2+4,6) + 32*C((n+1)/2+4,5) if n
 *            is odd Here C(n,k) is the binomial coefficient "n choose k". The
 *            binomial function is provided by the utils package
 * Range:     u8 n <= 2, u16 n <= 11, u32 n <= 75, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* n >= 1 by problem definition. */
    if ((n & 1) == 0) {          /* even n */
        idx_t m = n >> 1;        /* m = n/2 */
        seq_t t1 = binom(m + 6, 7);
        seq_t t2 = binom(m + 5, 6);
        return (seq_t)128 * t1 - (seq_t)64 * t2;
    } else {                     /* odd n */
        idx_t m = (n + 1) >> 1;  /* m = (n+1)/2  = (n+1)/2  */
        seq_t t1 = binom(m + 4, 7);
        seq_t t2 = binom(m + 4, 6);
        seq_t t3 = binom(m + 4, 5);
        return (seq_t)128 * t1 + (seq_t)128 * t2 + (seq_t)32 * t3;
    }
}
