/*
 * A382366  Expansion of 1/( 1 - 9 * Sum_{k>=0} x^(2^k) / (1 - x^(2^k))
 *          )^(1/3).
 * https://oeis.org/A382366
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 3, u64 n <= 19, u128 n <= 20,
 *            big any n
 * Helpers:   tzdx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A382366: Expansion of 1/(1-9*Σ_{k≥0} x^{2^k}/(1-x^{2^k}))^{1/3} */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/tzdx.h"

static void ensure_seq_capacity(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = *cap ? *cap : 1;
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(*arr, new_cap * sizeof(seq_t));
        *arr = tmp;
        *cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    static seq_t *a_vals = NULL, *b_vals = NULL, *conv = NULL;   /* a(n), B(n), convolution */
    static int64_t *w_vals = NULL;                               /* 9*(v2(k)+1)  */
    static size_t cap_a = 0, cap_b = 0, cap_conv = 0, cap_w = 0;
    static idx_t computed = 0;

    if (n < computed) return a_vals[n];   /* cache hit */

    /* extend capacities */
    ensure_seq_capacity(&a_vals, &cap_a, n + 1);
    ensure_seq_capacity(&b_vals, &cap_b, n + 1);
    ensure_seq_capacity(&conv,   &cap_conv, 2 * n + 1);
    ensure_seq_capacity((seq_t **)&w_vals, &cap_w, n + 1);

    /* initialization for first term */
    if (computed == 0) {
        a_vals[0] = 1;
        b_vals[0] = 1;
        conv[0] = 1;          /* a0*a0 */
        computed = 1;
    }

    /* compute new terms from computed to n */
    for (idx_t idx = computed; idx <= n; ++idx) {
        /* w[idx] = 9 * (1 + v2(idx)) */
        w_vals[idx] = 9LL * (1 + tzdx(idx));

        /* build b[idx] = Σ_{k=1..idx} w[k] * b[idx-k] */
        seq_t sum_b = 0;
        for (idx_t k = 1; k <= idx; ++k)
            sum_b += (seq_t)w_vals[k] * b_vals[idx - k];
        b_vals[idx] = sum_b;

        /* compute R = Σ_{k=1..idx} conv[k] * a[idx-k] */
        seq_t R = 0;
        for (idx_t k = 1; k <= idx; ++k)
            R += conv[k] * a_vals[idx - k];

        /* a[idx] = (b[idx] - R) / 3 */
        a_vals[idx] = (b_vals[idx] - R) / 3;

        /* update convolution conv[j+idx] for new a[idx] */
        seq_t a_idx = a_vals[idx];
        for (idx_t j = 0; j < idx; ++j) {
            conv[j + idx] += 2 * a_vals[j] * a_idx;
        }
        conv[2 * idx] += a_idx * a_idx;
    }

    computed = n + 1;
    return a_vals[n];
}
