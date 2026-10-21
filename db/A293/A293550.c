/*
 * A293550  a(n) = Sum_{k=0..n} k^3*binomial(2*n-k,n).
 * https://oeis.org/A293550
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0..n} k^3*binomial(2*n - k, n)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 62,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    /* initial binomial coefficient C(2*n, n) */
    seq_t b = binomial(2 * n, n);

    for (idx_t k = 0; k <= n; ++k)
    {
        seq_t kk  = (seq_t)k;
        seq_t k3  = kk * kk * kk;   /* k^3 */
        sum = sum + k3 * b;

        if (k == n)
            break;

        /* update b to binomial(2*n - (k+1), n) using
           C(m-1, n) = C(m, n) * (m-n)/m with m = 2*n - k */
        seq_t num = (seq_t)(n - k);            /* (m - n) = n - k */
        seq_t den = (seq_t)(2 * n - k);        /* m = 2*n - k */
        b = (b * num) / den;
    }

    return sum;
}
