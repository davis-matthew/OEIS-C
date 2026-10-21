/*
 * A258090  Expansion of q^(-5/6) * (eta(q) * eta(q^6)^2 / eta(q^3))^2 in
 *          powers of q.
 * https://oeis.org/A258090
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Euler transform of the periodic sequence (-2, -2, 0, -2,
 *            -2, -4) (period 6), with a(0)=1
 * Range:     i8 n <= 15, i16 n <= 635, i32 n <= 2500, i64 n <= 2500,
 *            i128 n <= 2500, big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_realloc.h"

/* periodic exponents e(n) used in the transform */
static const seq_t e_mod[6] = { (seq_t)-2, (seq_t)-2, (seq_t)0,
                                (seq_t)-2, (seq_t)-2, (seq_t)-4 };

/* Ensure *ptr has capacity >= needed; *cap holds current capacity. */
static inline void ensure_cap(size_t needed, size_t *cap, seq_t **ptr) {
    if (needed <= *cap) return;
    size_t newcap = (*cap ? (*cap << 1) : 1);
    while (newcap < needed) newcap <<= 1;
    seq_t *tmp = seq_realloc(*ptr, *cap, newcap);
    for (size_t i = *cap; i < newcap; ++i) tmp[i] = 0;
    *ptr = tmp;
    *cap = newcap;
}

seq_t a(idx_t n) {
    static seq_t *s_arr = NULL;
    static size_t s_cap = 0;
    static seq_t *a_arr = NULL;
    static size_t a_cap = 0;
    static size_t a_len = 0;  /* number of a values stored (a[0 .. a_len-1]) */

    size_t need = (size_t)n + 1;
    ensure_cap(need, &s_cap, &s_arr);
    ensure_cap(need, &a_cap, &a_arr);

    /* recompute divisor–weighted sums s[k] = Σ_{d|k} d * e(d) for 1 ≤ k ≤ n */
    for (idx_t k = 1; k <= n; ++k) s_arr[k] = 0;
    for (idx_t d = 1; d <= n; ++d) {
        seq_t inc = (seq_t)d * e_mod[(d - 1) % 6];
        for (idx_t m = d; m <= n; m += d) s_arr[m] += inc;
    }

    if (a_len == 0) {  /* initialise a(0) */
        a_arr[0] = (seq_t)1;
        a_len = 1;
    }

    /* compute missing a values up to n */
    for (idx_t i = a_len; i <= n; ++i) {
        seq_t sum = 0;
        for (idx_t k = 1; k <= i; ++k) sum += s_arr[k] * a_arr[i - k];
        a_arr[i] = sum / (seq_t)i;   /* division is exact */
    }
    a_len = n + 1;
    return a_arr[n];
}
