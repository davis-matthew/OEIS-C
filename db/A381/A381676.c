/*
 * A381676  a(n) = Sum_{k=0..n} binomial(n,k) * ( binomial(n,k) -
 *          binomial(n,k-1) )^2.
 * https://oeis.org/A381676
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k) * ( binomial(n,k) -
 *            binomial(n,k-1) )^2
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 24, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    seq_t sum   = 0;
    seq_t b_prev = 0; /* binomial(n,-1) = 0 */
    seq_t b = 1;      /* placeholder for binomial(n,k) */

    for (idx_t k = 0; k <= n; ++k) {
        if (k > 0) {
            /* Compute binomial(n,k) from binomial(n,k-1) */
            seq_t num = b_prev * (seq_t)(n - k + 1);
            b = num / (seq_t)k;          /* exact division */
        } else {
            b = 1;                       /* binomial(n,0) */
        }

        /* Absolute difference between consecutive binomials */
        seq_t diff;
        if (b >= b_prev) {
            diff = b - b_prev;
        } else {
            diff = b_prev - b;
        }

        seq_t diff2 = diff * diff;
        seq_t term   = b * diff2;
        sum += term;

        b_prev = b;
    }

    return sum;
}
