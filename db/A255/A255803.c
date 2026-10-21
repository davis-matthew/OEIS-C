/*
 * A255803  G.f.: Product_{k>=1} 1/(1-x^k)^(3*k+2).
 * https://oeis.org/A255803
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = coefficient of xⁿ in ∏_{k≥1} 1/(1−xᵏ)^(3k+2)
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 23, u64 n <= 61, u128 n <= 166,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    size_t N = (size_t)n;
    seq_t *dp_old = (seq_t*)calloc(N + 1, sizeof(seq_t));
    seq_t *dp_new = (seq_t*)malloc((N + 1) * sizeof(seq_t));
    seq_t *weight = (seq_t*)calloc(N + 1, sizeof(seq_t));

    dp_old[0] = (seq_t)1;

    for (idx_t k = 1; k <= n; ++k) {
        size_t max_t = n / k;

        /* pre‑compute the binomial weights for this part size */
        weight[0] = (seq_t)1;
        for (size_t t = 1; t <= max_t; ++t) {
            seq_t mult = weight[t - 1] * (seq_t)(3 * k + t + 1);
            weight[t]   = mult / (seq_t)t;
        }

        /* start with zero vector for new DP values */
        memset(dp_new, 0, (N + 1) * sizeof(seq_t));

        for (size_t t = 0; t <= max_t; ++t) {
            seq_t w = weight[t];
            idx_t step = t * k;
            for (idx_t sum = step; sum <= n; ++sum) {
                dp_new[sum] += dp_old[sum - step] * w;
            }
        }

        /* swap the DP arrays for the next iteration */
        seq_t *tmp = dp_old;
        dp_old = dp_new;
        dp_new = tmp;
    }

    seq_t result = dp_old[N];

    free(dp_old);
    free(dp_new);
    free(weight);

    return result;
}
