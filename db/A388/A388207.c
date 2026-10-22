/*
 * A388207  a(n) = Sum_{k=0..n} 4^k * binomial(n,k) * binomial(n+4,k).
 * https://oeis.org/A388207
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} 4^k * binomial(n,k) * binomial(n+4,k)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Start with the k = 0 term */
    seq_t sum     = 1;   /* 4^0 * C(n,0) * C(n+4,0) = 1 */
    seq_t binom_nk  = 1; /* C(n,0)          */
    seq_t binom_n4k = 1; /* C(n+4,0)        */
    seq_t pow4k    = 1;  /* 4^0             */

    /* Compute the remaining terms k = 1 .. n */
    for (idx_t k = 1; k <= n; ++k)
    {
        /* Increment the power of 4 */
        pow4k *= 4;

        /* Update C(n,k) from C(n,k-1) */
        seq_t num_n = (seq_t)(n - k + 1) * binom_nk;  /* numerator = (n-k+1) * C(n,k-1) */
        binom_nk    = num_n / k;   /* exact division */

        /* Update C(n+4,k) from C(n+4,k-1) */
        seq_t num_n4 = (seq_t)(n + 4 - k + 1) * binom_n4k; /* = (n+5-k) * C(n+4,k-1) */
        binom_n4k    = num_n4 / k;   /* exact division */

        /* Compute the current term: 4^k * C(n,k) * C(n+4,k) */
        seq_t term = pow4k;
        term *= binom_nk;
        term *= binom_n4k;

        /* Add to running sum */
        sum += term;
    }

    return sum;
}
