/*
 * A385286  a(n) = (n!)^2 [x^n] hypergeom([], [1], x)^8.
 * https://oeis.org/A385286
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk
 * Formula:   a(n) = (n!)^2 * [x^n] (I0(2*sqrt(x)))^8
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A385286: sum of squares of multinomial coefficients for 8 parts */
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

#define MAX_CATEGORIES 8

/* Helper for small n (<=12) – exhaustive enumeration works for 64‑bit builds */
static seq_t small_factorial[13];
static bool small_fact_inited = false;

static void init_small_factorial(void)
{
    if (small_fact_inited) return;
    small_fact_inited = true;
    small_factorial[0] = (seq_t)1;
    for (idx_t i = 1; i < 13; ++i)
        small_factorial[i] = small_factorial[i-1] * (seq_t)i;
}

/* Compute a(n) for large n using dynamic programming */
static seq_t *dp_cur = NULL;
static seq_t *dp_next = NULL;
static seq_t *cached_vals = NULL;
static idx_t cached_max = 0;

static void recompute_dp(idx_t n)
{
    /* free old arrays if any */
    if (cached_vals)  free(cached_vals);
    if (dp_cur)       free(dp_cur);
    if (dp_next)      free(dp_next);

    size_t sz = (size_t)(n + 1);
    cached_vals = (seq_t*)malloc(sz * sizeof(seq_t));
    dp_cur      = (seq_t*)malloc(sz * sizeof(seq_t));
    dp_next     = (seq_t*)malloc(sz * sizeof(seq_t));
    if (!cached_vals || !dp_cur || !dp_next) return;

    /* initialize for 0 categories */
    for (size_t i = 0; i < sz; ++i) dp_cur[i] = 0;
    dp_cur[0] = (seq_t)1;

    /* iterate over 8 categories */
    for (int cat = 1; cat <= MAX_CATEGORIES; ++cat) {
        for (size_t i = 0; i < sz; ++i) {
            seq_t sum = 0;
            seq_t C   = 1;
            for (size_t k = 0; k <= i; ++k) {
                seq_t term = dp_cur[i - k];
                if (term) {
                    seq_t sq = C * C;
                    sum += term * sq;
                }
                if (k < i) {
                    C = C * (seq_t)(i - k) / (seq_t)(k + 1);
                }
            }
            dp_next[i] = sum;
        }
        /* swap current and next */
        seq_t *tmp = dp_cur;
        dp_cur    = dp_next;
        dp_next   = tmp;
    }
    /* copy result into cache */
    for (size_t i = 0; i < sz; ++i) cached_vals[i] = dp_cur[i];
    cached_max = n;
}

seq_t a(idx_t n)
{
    /* Use enumeration for n <= 12 – safe for 64‑bit builds */
    if (n <= 12) {
        init_small_factorial();
        seq_t sum = 0;
        /* 8 nested loops – n is small, so this is fine */
        for (idx_t k0 = 0; k0 <= n; ++k0)
        for (idx_t k1 = 0; k0 + k1 <= n; ++k1)
        for (idx_t k2 = 0; k0 + k1 + k2 <= n; ++k2)
        for (idx_t k3 = 0; k0 + k1 + k2 + k3 <= n; ++k3)
        for (idx_t k4 = 0; k0 + k1 + k2 + k3 + k4 <= n; ++k4)
        for (idx_t k5 = 0; k0 + k1 + k2 + k3 + k4 + k5 <= n; ++k5)
        for (idx_t k6 = 0; k0 + k1 + k2 + k3 + k4 + k5 + k6 <= n; ++k6) {
            idx_t k7 = n - (k0 + k1 + k2 + k3 + k4 + k5 + k6);
            seq_t term = small_factorial[n];
            term /= small_factorial[k0];
            term /= small_factorial[k1];
            term /= small_factorial[k2];
            term /= small_factorial[k3];
            term /= small_factorial[k4];
            term /= small_factorial[k5];
            term /= small_factorial[k6];
            term /= small_factorial[k7];
            term = term * term;
            sum += term;
        }
        return sum;
    }

    /* For larger n, use DP method (works for arbitrary‑precision builds) */
    if (cached_vals == NULL || n > cached_max)
        recompute_dp(n);

    return cached_vals[n];
}
