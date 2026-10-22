/*
 * A361878  a(n) = hypergeom([-n, -n, n, n + 1], [1, 1, 1], 1).
 * https://oeis.org/A361878
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{k=0..n} binomial(n,k)^2 * binomial(n+k,k) *
 *            binomial(n+k-1,k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* start with k = 0 term: 1 */
    seq_t sum = 1;
    seq_t b1 = 1;   /* binomial(n,k) */
    seq_t b2 = 1;   /* binomial(n+k,k)  */
    seq_t b3 = 1;   /* binomial(n+k-1,k) */

    for (idx_t k = 1; k <= n; ++k) {
        /* update binom(n,k)   = binom(n,k-1) * (n-k+1) / k */
        b1 = b1 * ((seq_t)(n - k + 1)) / (seq_t)k;

        /* update binom(n+k,k) = binom(n+k-1,k-1) * (n+k) / k */
        b2 = b2 * ((seq_t)(n + k)) / (seq_t)k;

        /* update binom(n+k-1,k) = binom(n+k-2,k-1) * (n+k-1) / k */
        b3 = b3 * ((seq_t)(n + k - 1)) / (seq_t)k;

        seq_t term = b1 * b1;  /* binomial(n,k)^2 */
        term = term * b2;      /* * binomial(n+k,k) */
        term = term * b3;      /* * binomial(n+k-1,k) */
        sum += term;
    }

    return sum;
}
