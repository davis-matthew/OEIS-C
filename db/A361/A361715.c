/*
 * A361715  a(n) = Sum_{k = 0..n-1} binomial(n,k)^2*binomial(n+k-1,k).
 * https://oeis.org/A361715
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 20, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = Sum_{k = 0 .. n-1} binomial(n,k)^2 * binomial(n+k-1,k)
 * Offset: 0   (a(0) = 0)
 *
 * The function uses only arithmetic on seq_t and does not rely on any
 * external libraries beyond the ones provided in the build environment.
 */

#include <stdlib.h>  /* for standard types */

seq_t a(idx_t n)
{
    /* a(0) is defined as 0 */
    if (n == 0)
        return 0;

    seq_t sum = 0;

    /* C(n,0) = 1 */
    seq_t binom_nk = 1;
    /* C(n-1,0) = 1  (k=0 term of the second binomial) */
    seq_t binom_npk_k = 1;

    for (idx_t k = 0; k < n; ++k)
    {
        /* term = C(n,k)^2 * C(n + k - 1, k) */
        seq_t term = binom_nk * binom_nk * binom_npk_k;
        sum += term;

        /* Update binomials for next k (k+1) */
        /* C(n,k+1) = C(n,k) * (n - k) / (k + 1) */
        binom_nk = (binom_nk * (seq_t)(n - k)) / (k + 1);

        /* C(n + k, k+1) = C(n + k - 1, k) * (n + k) / (k + 1) */
        binom_npk_k = (binom_npk_k * (seq_t)(n + k)) / (k + 1);
    }

    return sum;
}
