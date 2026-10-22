/*
 * A316405  Number of multisets of exactly four nonempty binary words with a
 *          total of n letters such that no word has a majority of 0's.
 * https://oeis.org/A316405
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n y^4 in ∏_{j≥1} (1 - y
 *            x^j)^{-A027306(j)}. The sequence A027306 is 2^{j‑1} for odd j,
 *            and 2^{j‑1} + ½·C(j, j/2) for even j
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 28, u64 n <= 57, u128 n <= 118,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* External integer types: defined by the build system.
   Do not redefine them here. */
typedef seq_t (*seq_t_func)(idx_t);
typedef idx_t (*idx_t_func)(idx_t);

seq_t a(idx_t n)
{
    if (n < 4u) return (seq_t)0;            /* offset 4, undefined below */

    /* width = n + 1, dp size = 5 * width (for 0..4 words) */
    const idx_t width = n + 1u;
    const size_t dp_size = 5ul * (size_t)width;
    seq_t *dp = (seq_t *)calloc(dp_size, sizeof(seq_t));
    if (!dp) return (seq_t)0;                /* allocation failure */

    dp[0] = (seq_t)1;                       /* dp[0][0] = 1 */

    /* variables for A027306(j) calculation */
    seq_t power_of_two = (seq_t)1;           /* 2^0 initially */
    seq_t half_c = (seq_t)0;                 /* C(2k, k) for current even j */
    idx_t k_even = 0;                        /* k = j/2 for even j */

    /* temporary array for binomial coefficients C(f+m-1, m), m=1..4 */
    seq_t comb[5];
    comb[0] = (seq_t)0;

    for (idx_t j = 1; j <= n; ++j)
    {
        power_of_two <<= 1;                  /* power_of_two = 2^j */

        /* compute f = A027306(j) */
        seq_t f;
        if (j & 1u)                          /* odd j : f = 2^{j-1} */
        {
            f = power_of_two >> 1u;
        }
        else                                 /* even j */
        {
            ++k_even;                        /* k = j/2 */
            if (k_even == 1u)                /* C(2,1) = 2 */
            {
                half_c = (seq_t)2;
            }
            else
            {
                /* C(2k, k) = C(2k-2, k-1) * (2k)(2k-1) / (k^2) */
                seq_t num = half_c;
                num *= (seq_t)(2u * k_even);
                num *= (seq_t)(2u * k_even - 1u);
                seq_t den = (seq_t)(k_even) * (seq_t)(k_even);
                half_c = num / den;
            }
            f = (power_of_two >> 1u) + (half_c >> 1u);
        }

        /* compute multinomial coefficients for m = 1..4 */
        comb[1] = f;
        comb[2] = f * (f + (seq_t)1u) / (seq_t)2u;
        comb[3] = comb[2] * (f + (seq_t)2u) / (seq_t)3u;
        comb[4] = comb[3] * (f + (seq_t)3u) / (seq_t)4u;

        /* DP update: iterate t from 4 down to 0 (but skip t=4) */
        for (int t = 3; t >= 0; --t)
        {
            const idx_t base_t = (idx_t)t * width;
            seq_t *dp_t = dp + base_t;
            const int max_add = 4 - t;
            for (idx_t s = 0; s <= n; ++s)
            {
                seq_t cur = dp_t[s];
                if (!cur) continue;
                idx_t remaining = n - s;
                idx_t max_add_by_len = remaining / j;
                int m_max = max_add;
                if (max_add_by_len < (idx_t)m_max) m_max = (int)max_add_by_len;
                for (int m = 1; m <= m_max; ++m)
                {
                    idx_t ns = s + (idx_t)m * j;
                    dp[(t + m) * width + ns] += cur * comb[m];
                }
            }
        }
    }

    seq_t result = dp[4u * width + n];
    free(dp);
    return result;
}
