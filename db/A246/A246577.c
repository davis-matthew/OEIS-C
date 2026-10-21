/*
 * A246577  G.f.: (Product_{r>=1} (1 - x^r))*x^(k^2)/Product_{i=1..k} ((1 -
 *          x^i)^2) with k=3.
 * https://oeis.org/A246577
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coeff of x^n in (Product_{r>=1} (1 - x^r)) * x^9 / ((1 -
 *            x)^2 (1 - x^2)^2 (1 - x^3)^2)
 * Range:     i8 n <= 49, i16 n <= 64, i32 n <= 64, i64 n <= 64, i128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses the recurrence
   a(n) = - Σ_{j=1}^{12} q_j * a(n-j) + e_{n-9},
   where q_j are coefficients of Q(x) = (1-x)^2(1-x^2)^2(1-x^3)^2
   and e_m are coefficients of the Euler product. */

#include <stdlib.h>

static seq_t *a_cache = NULL;   /* sequence values */
static size_t a_cap = 0;
static seq_t *e_cache = NULL;   /* Euler product coefficients up to some index */
static size_t e_cap = 0;

/* Ensure that e_cache[0..up_to] is populated using the pentagonal number theorem. */
static void ensure_e(size_t up_to)
{
    /* allocate space if necessary (additive extension keeps old values) */
    if (e_cap <= up_to) {
        size_t new_cap = up_to + 1;
        seq_t *new_e = (seq_t*)realloc(e_cache, new_cap * sizeof(seq_t));
        if (!new_e) exit(1);
        e_cache = new_e;
        e_cap   = new_cap;
    }

    /* recompute all values up to up_to from scratch (fast enough for n≤10000) */
    for (size_t i = 0; i <= up_to; ++i) e_cache[i] = 0;
    e_cache[0] = 1;   /* one term of the product */

    for (idx_t k = 1; ; ++k) {
        idx_t g1 = k * ((idx_t)3 * k - 1) / 2;
        idx_t g2 = k * ((idx_t)3 * k + 1) / 2;
        if (g1 > up_to && g2 > up_to) break;
        seq_t sign = (k % 2 == 0) ? (seq_t)1 : (seq_t)-1;
        if (g1 <= up_to) e_cache[g1] += sign;
        if (g2 <= up_to) e_cache[g2] += sign;
    }
}

/* Ensure that a_cache[0..up_to] is computed. */
static void ensure_a(size_t up_to)
{
    if (a_cap <= up_to) {
        size_t old_cap = a_cap;
        size_t new_cap = up_to + 1;
        seq_t *new_a = (seq_t*)realloc(a_cache, new_cap * sizeof(seq_t));
        if (!new_a) exit(1);
        a_cache = new_a;

        /* initialize new entries to zero (although they'll be overwritten) */
        for (size_t i = old_cap; i < new_cap; ++i) a_cache[i] = 0;

        /* coefficients q_j of Q(x) = (1-x)^2(1-x^2)^2(1-x^3)^2 */
        const int q[13] = {1,-2,-1,2,3,0,-6,0,3,2,-1,-2,1};

        for (size_t i = old_cap; i <= up_to; ++i) {
            seq_t sum = 0;
            for (int j = 1; j <= 12; ++j) {
                if (i >= (size_t)j) sum += (seq_t)q[j] * a_cache[i - j];
            }
            seq_t right = 0;
            if (i >= 9) right = e_cache[i - 9];
            a_cache[i] = -sum + right;
        }
        a_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    size_t idx = (size_t)n;
    size_t need_e = (idx >= 9) ? idx - 9 : 0;
    ensure_e(need_e);
    ensure_a(idx);
    return a_cache[idx];
}
