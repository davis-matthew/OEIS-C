/*
 * A349695  a(n) = (2n^2 - n + 2) * (2n)! / ((n + 1) * (n + 2) * n!^2).
 * https://oeis.org/A349695
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2n^2 - n + 2) * (2n)! / ((n + 1) * (n + 2) * (n!)^2)
 *            (equivalently a(n) = Catalan(n) * (2n^2 - n + 2)/(n + 2) )
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Compute binomial coefficient binom(2n,n) multiplicatively. */
static seq_t binom_2n_n(idx_t n)
{
    seq_t res = (seq_t)1;
    for (idx_t k = 1; k <= n; ++k)
    {
        /* res = res * (n + k) / k;
           Both intermediate results are integer. */
        res = res * ((seq_t)n + (seq_t)k) / (seq_t)k;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;           /* offset is 1, so ignore n=0 */

    /* Compute Catalan number C_n = binom(2n,n) / (n+1). */
    seq_t binom = binom_2n_n(n);
    seq_t catalan = binom / ((seq_t)n + (seq_t)1);

    /* Compute the remaining factor (2n^2 - n + 2)/(n+2). */
    seq_t two_n_sq = (seq_t)2 * (seq_t)n * (seq_t)n;      /* 2*n^2 */
    seq_t numerator = two_n_sq - (seq_t)n + (seq_t)2;      /* 2n^2 - n + 2 */
    seq_t denominator = (seq_t)n + (seq_t)2;                /* n + 2 */

    /* Final result: catalan * numerator / denominator.  Exact division. */
    seq_t result = catalan * numerator / denominator;
    return result;
}
