/*
 * A383356  a(n) = index of the smallest nonagonal number having the same
 *          digital sum as the n-th triangular number.
 * https://oeis.org/A383356
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = index of the smallest nonagonal number having the same *
 *            digital sum as the n-th triangular number
 * Range:     u8 n <= 11, u16 n <= 952, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/digit_sum.h"

seq_t a(idx_t n)
{
    /* compute the n-th triangular number T_n = n*(n+1)/2 */
    seq_t tri = (seq_t)n * ((seq_t)n + 1) / 2;

    /* digital sum of T_n, cast to idx_t for array indexing */
    idx_t d = (idx_t)digit_sum(tri);

    /* static cache mapping a digital sum to the minimal index k.
     * 0 indicates that entry hasn't been computed yet. */
    static idx_t min_k[2001] = {0};

    /* Return cached answer if available. */
    if (d <= 2000 && min_k[d] != 0)
        return (seq_t)min_k[d];

    /* Search for the smallest k such that digit_sum(N_k) == d. */
    for (idx_t k = 1;; ++k)
    {
        /* Nonagonal number N_k = (7k^2 - 5k)/2. */
        seq_t nk = ((seq_t)7 * k * k - (seq_t)5 * k) / 2;

        if ((idx_t)digit_sum(nk) == d)   /* match found */
        {
            if (d <= 2000)
                min_k[d] = k;           /* cache for future queries */
            return (seq_t)k;
        }
    }
}
