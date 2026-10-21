/*
 * A241275  a(n) = 6*a(n-1) + 5*(2^(n-1)-1) for n > 0, a(0) = 0.
 * https://oeis.org/A241275
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = 6*a(n-1) + 5*(2^(n-1)-1) for n>0, a(0)=0 */
#include <stdlib.h>   /* for size_t, if needed */

/* compute the nth term of OEIS A241275 */
seq_t a(idx_t n)
{
    /* handle the base case */
    if (n == 0)
        return (seq_t)0;

    seq_t s    = (seq_t)0;   /* a(0) */
    seq_t pow2 = (seq_t)1;   /* 2^(k-1) for the current k (starts with k=1) */

    for (idx_t k = 1; k <= n; ++k) {
        /* term = 2^(k-1) - 1  (always fits in seq_t because 2^(k-1) <= pow2) */
        seq_t term = pow2 - (seq_t)1;

        /* a(k) = 6*a(k-1) + 5*term */
        s = (seq_t)6 * s + (seq_t)5 * term;

        /* advance pow2 to 2^k for the next iteration */
        pow2 = pow2 * (seq_t)2;
    }

    return s;
}
