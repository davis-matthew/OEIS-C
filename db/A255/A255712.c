/*
 * A255712  Number of length n 1..(4+2) arrays with no leading partial sum
 *          equal to a prime and no consecutive values equal.
 * https://oeis.org/A255712
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 17, u64 n <= 34, u128 n <= 66,
 *            big any n
 * Helpers:   make_prime_table
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A255712: number of length‑n arrays over {1..6}
   with no two consecutive equal entries and no prefix sum equal to a prime.
   Computed by dynamic programming over prefix sums. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/make_prime_table.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Maximum possible prefix sum */
    idx_t max_sum = 6 * n;

    /* Prime table up to max_sum */
    bool *is_prime = (bool *)calloc((size_t)(max_sum + 1), sizeof(bool));
    if (!is_prime)
        return 0;
    make_prime_table(max_sum, is_prime);

    /* Allocate two layers: 6 last values × (max_sum+1) sums */
    const size_t layer_size = (size_t)(max_sum + 1);
    seq_t **dp_cur = (seq_t **)malloc(6 * sizeof(seq_t *));
    seq_t **dp_next = (seq_t **)malloc(6 * sizeof(seq_t *));
    if (!dp_cur || !dp_next) {
        free(is_prime);
        return 0;
    }
    for (int v = 0; v < 6; ++v) {
        dp_cur[v]  = (seq_t *)calloc(layer_size, sizeof(seq_t));
        dp_next[v] = (seq_t *)calloc(layer_size, sizeof(seq_t));
        if (!dp_cur[v] || !dp_next[v]) {
            free(is_prime);
            for (int w = 0; w <= v; ++w) {
                free(dp_cur[w]);
                free(dp_next[w]);
            }
            free(dp_cur);
            free(dp_next);
            return 0;
        }
    }

    /* Base: length 1 */
    for (int val = 1; val <= 6; ++val)
        if (!is_prime[ (size_t)val ])
            dp_cur[val - 1][ (size_t)val ] = 1;

    /* Dynamic programming for lengths 2 … n */
    for (idx_t step = 2; step <= n; ++step) {
        /* Clear next layer */
        for (int v = 0; v < 6; ++v)
            memset(dp_next[v], 0, layer_size * sizeof(seq_t));

        idx_t prev_max = 6 * (step - 1);

        /* For every reachable prefix sum */
        for (idx_t s = 1; s <= prev_max; ++s) {
            /* Sum over all last values for this sum */
            seq_t tot = 0;
            for (int v = 0; v < 6; ++v)
                tot += dp_cur[v][ (size_t)s ];
            if (!tot)
                continue;

            /* Distribute to next sums with each possible new value */
            for (int v = 0; v < 6; ++v) {
                idx_t ns = s + (idx_t)(v + 1);
                if (ns > max_sum || is_prime[ (size_t)ns ])
                    continue;
                /* Avoid consecutive equal values: subtract dp_cur[v][s] */
                dp_next[v][ (size_t)ns ] += (tot - dp_cur[v][ (size_t)s ]);
            }
        }

        /* Swap current and next layers */
        seq_t **tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;
    }

    /* Accumulate result from final layer */
    seq_t result = 0;
    for (int v = 0; v < 6; ++v)
        for (idx_t s = n; s <= max_sum; ++s)
            if (!is_prime[ (size_t)s ])
                result += dp_cur[v][ (size_t)s ];

    /* Cleanup */
    for (int v = 0; v < 6; ++v) {
        free(dp_cur[v]);
        free(dp_next[v]);
    }
    free(dp_cur);
    free(dp_next);
    free(is_prime);

    return result;
}
