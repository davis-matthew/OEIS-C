/*
 * A320243  Number of integer solutions (a_1, a_2, ... , a_8) to the equation
 *          a_1^2 + 2*a_2^2 + ... + 8*a_8^2 = 3*n.
 * https://oeis.org/A320243
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integer solutions (a₁,…,a₈) to * a₁² + 2*a₂² +
 *            … + 8*a₈² = 3·n * Implementation: dynamic‑programming
 *            convolution over the * weighted squares. For each k = 1…8 we
 *            convolve * the current distribution with the pattern for * m =
 *            0, ±1, ±2, … The result contains the counts * for every...
 * Range:     u8 n <= 4, u16 n <= 30, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* cached distribution of weighted sums */
static seq_t *dp = NULL;
/* largest sum for which dp is valid (max_sum = 3 * current_n) */
static idx_t dp_max_sum = 0;

/* compute the distribution for all sums up to 3*n */
static void compute_dp(idx_t n)
{
    idx_t max_sum = 3 * n;
    /* allocate new array */
    seq_t *new_dp = (seq_t*)calloc((size_t)(max_sum + 1), sizeof(seq_t));
    if (!new_dp) abort();
    new_dp[0] = (seq_t)1;           /* zero sum – one empty solution */

    /* for each weight k */
    for (idx_t k = 1; k <= 8; ++k)
    {
        /* temporary array for the convolution */
        seq_t *tmp = (seq_t*)calloc((size_t)(max_sum + 1), sizeof(seq_t));
        if (!tmp) { free(new_dp); abort(); }

        /* perform the convolution */
        for (idx_t s = 0; s <= max_sum; ++s)
        {
            seq_t ways = new_dp[s];
            if (ways == 0) continue;

            /* m = 0 contributes weight 0 */
            tmp[s] += ways;

            /* m = ±1, ±2, … contribute weight k*m² (two signs) */
            for (idx_t m = 1; ; ++m)
            {
                idx_t w  = (idx_t)k * m * m;
                idx_t ns = s + w;
                if (ns > max_sum) break;
                tmp[ns] += ways * 2;
            }
        }

        free(new_dp);
        new_dp = tmp;
    }

    /* store the result */
    free(dp);
    dp       = new_dp;
    dp_max_sum = max_sum;
}

seq_t a(idx_t n)
{
    /* recompute if we don't yet have enough sums */
    if (!dp || 3 * n > dp_max_sum)
        compute_dp(n);

    return dp[3 * n];
}
