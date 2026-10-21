/*
 * A280719  Expansion of (Sum_{k>=0} x^(k*(2*k-1)))^6.
 * https://oeis.org/A280719
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in (∑_{k≥0} x^{k(2k-1)})^6
 * Range:     u8 n <= 36, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of ordered 6‑tuples of hexagonal numbers sum to n. */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* Allocate DP arrays for convolution */
    size_t len = (size_t)n + 1U;
    seq_t *dp_cur  = (seq_t*)calloc(len, sizeof(seq_t));
    seq_t *dp_next = (seq_t*)calloc(len, sizeof(seq_t));
    if (!dp_cur || !dp_next) {
        free(dp_cur); free(dp_next);
        return 0;
    }

    /* Build list of hexagonal numbers ≤ n */
    idx_t *hex = (idx_t*)calloc(len, sizeof(idx_t));
    if (!hex) {
        free(dp_cur); free(dp_next);
        return 0;
    }
    idx_t hex_count = 0;
    for (idx_t k = 0;; ++k) {
        idx_t h = k * (2 * k - 1);
        if (h > n) break;
        hex[hex_count++] = h;
    }

    /* Base: one way to form sum 0 with zero terms */
    dp_cur[0] = 1;

    /* Convolve six times, one for each hexagonal summand */
    for (idx_t t = 0; t < 6; ++t) {
        /* Zero next DP array */
        memset(dp_next, 0, len * sizeof(seq_t));

        /* For each available hexagonal number, add contributions */
        for (idx_t i = 0; i < hex_count; ++i) {
            idx_t h = hex[i];
            for (idx_t s = h; s <= n; ++s) {
                dp_next[s] += dp_cur[s - h];
            }
        }

        /* Swap pointers for next iteration */
        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;
    }

    seq_t result = dp_cur[n];

    /* Clean up */
    free(dp_cur);
    free(dp_next);
    free(hex);

    return result;
}
