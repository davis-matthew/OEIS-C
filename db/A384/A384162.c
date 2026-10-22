/*
 * A384162  Number of length n words over an n-ary alphabet such that a single
 *          letter in every run of letters is marked.
 * https://oeis.org/A384162
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = [x^n] n*x/(1 - x*(1 - x + n))
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* Allow calls with n == 0 (out of range) */
    if (n == 0) return (seq_t)0;

    /* Static cache for intermediate arrays to support multiple calls */
    static idx_t max_alloc = 0;
    static seq_t *prev = NULL;
    static seq_t *cur  = NULL;
    static seq_t *a_arr = NULL;
    static seq_t *power = NULL;

    /* Resize arrays if the requested n is larger than current capacity */
    if (n > max_alloc) {
        free(prev);
        free(cur);
        free(a_arr);
        free(power);
        size_t sz = (size_t)(n + 1);
        prev  = (seq_t *)malloc(sz * sizeof(seq_t));
        cur   = (seq_t *)malloc(sz * sizeof(seq_t));
        a_arr = (seq_t *)malloc(sz * sizeof(seq_t));
        power = (seq_t *)malloc(sz * sizeof(seq_t));
        max_alloc = n;
    }

    /* Initialize arrays to zero up to n */
    memset(prev,  0, (size_t)(n + 1) * sizeof(seq_t));
    memset(cur,   0, (size_t)(n + 1) * sizeof(seq_t));
    memset(a_arr, 0, (size_t)(n + 1) * sizeof(seq_t));
    memset(power, 0, (size_t)(n + 1) * sizeof(seq_t));

    /* power[i] = 1 for all i (since (n‑1)^0 = 1) */
    for (idx_t i = 1; i <= n; ++i)
        power[i] = (seq_t)1;

    /* k = 1 : f[n][1] = n, contribution a[n] += n*n */
    for (idx_t i = 1; i <= n; ++i) {
        prev[i] = (seq_t)i;                /* f[i][1] */
        a_arr[i] += (seq_t)i * prev[i];    /* i * i */
    }

    /* Update powers for the next k (k = 2): multiply by (i‑1) */
    for (idx_t i = 1; i <= n; ++i)
        power[i] *= (seq_t)(i - 1);

    /* Main loop over k = 2 .. n */
    for (idx_t k = 2; k <= n; ++k) {
        /* Incremental computation of f[n][k] using only prev array */
        seq_t sum_h = 0, sum_ph = 0;
        for (idx_t i = k; i <= n; ++i) {
            seq_t prev_val = prev[i - 1];
            sum_h  += prev_val;
            sum_ph += prev_val * (seq_t)(i - 1);
            cur[i] = (seq_t)i * sum_h - sum_ph;   /* f[i][k] */
        }

        /* Add contributions for this k */
        for (idx_t i = k; i <= n; ++i) {
            seq_t contrib = (seq_t)i * power[i];
            contrib *= cur[i];
            a_arr[i] += contrib;
        }

        /* Update powers for the next k */
        for (idx_t i = k; i <= n; ++i)
            power[i] *= (seq_t)(i - 1);

        /* Prepare for next iteration: prev becomes cur */
        seq_t *tmp = prev;
        prev = cur;
        cur  = tmp;
    }

    return a_arr[n];
}
