/*
 * A249371  Expansion of q^3 * f(q) * f(-q^4) * f(q^15) * f(-q^60) * chi(-q^3)
 *          * chi(-q^5) in powers of q where chi(), f() are Ramanujan theta
 *          functions.
 * https://oeis.org/A249371
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 36, i16 n <= 999, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A249371 – Euler transform of the period‑30 sequence
 *
 *  The generating function is
 *      G(q) = ∏_{k≥1} (1‑q^k)^(–c(k‑1))   with
 *          c = [1, –2, 0, –2, 0, –2, 1, –2, 0, –2, 1, –2, 1, –2,
 *               0, –2, 1, –2, 1, –2, 0, –2, 1, –2, 0, –2, 0, –2, 1, –4]
 *      (period 30, indexed from 0).
 *
 *  Recurrence:
 *      a(3) = 1
 *      a(n) = (1/(n‑3)) · Σ_{k=1}^{n‑3} B(k) · a(n‑3 – k)   (n ≥ 4)
 *      where B(k) = Σ_{d|k} d · c(d‑1).
 *
 *  The implementation stores the recurrence values for indices
 *  starting at 0 corresponding to OEIS index 3 and uses memoisation
 *  so that repeated calls in any order are fast.
 */

#include <stddef.h>
#include "../utils/seq_alloc.h"     /* seq_alloc  */
#include "../utils/seq_realloc.h"   /* seq_realloc */

/* period‑30 fragment of c(k), indexed from 0 */
static const int period_c[30] = {
    1, -2, 0, -2, 0, -2, 1, -2, 0, -2,
    1, -2, 1, -2, 0, -2, 1, -2, 1, -2,
    0, -2, 1, -2, 0, -2, 0, -2, 1, -4
};

static seq_t *a_cache    = NULL;      /* recurrence values a_seq[i] (i ≥ 0) */
static seq_t *coef_cache = NULL;      /* B(k) for k ≥ 1 */
static idx_t  cache_size = 0;         /* allocated array size (≥ 1) */
static idx_t  last_computed = 0;      /* highest recurrence index computed */

/* ------------------------------------------------------------------ */
/* ensure the internal arrays are of size ≥ n+1; initialise new cells */
static void ensure_capacity(idx_t n)
{
    if (cache_size > n)
        return;
    size_t new_cap = (size_t)n + 1;    /* capacity needed */

    /* grow a_cache */
    if (!a_cache)
        a_cache = seq_alloc(new_cap);
    else
        a_cache = seq_realloc(a_cache, cache_size, new_cap);

    /* grow coef_cache */
    if (!coef_cache)
        coef_cache = seq_alloc(new_cap);
    else
        coef_cache = seq_realloc(coef_cache, cache_size, new_cap);

    /* initialise new entries to 0 (they will be overwritten later) */
    for (idx_t i = cache_size; i <= n; ++i) {
        a_cache[i]    = 0;
        coef_cache[i] = 0;
    }
    cache_size = new_cap;
}

/* ------------------------------------------------------------------ */
/* compute B(k) for all k ≤ n   (k ≥ 1)                               */
static void recompute_coef(idx_t n)
{
    /* zero all coefficients (index 0 is unused) */
    for (idx_t i = 1; i <= n; ++i)
        coef_cache[i] = 0;

    for (idx_t d = 1; d <= n; ++d) {
        int cd = period_c[(d - 1) % 30];
        if (cd == 0)
            continue;
        seq_t inc = (seq_t)d * (seq_t)cd;
        for (idx_t m = d; m <= n; m += d)
            coef_cache[m] += inc;
    }
}

/* ------------------------------------------------------------------ */
/* main sequence function                                              */
seq_t a(idx_t n)
{
    if (n < 3)                 /* below the offset – undefined */
        return 0;

    idx_t target = n - 3;     /* recurrence index corresponding to OEIS n */

    ensure_capacity(target);

    if (target == 0) {        /* base case a(3) = 1 */
        a_cache[0] = 1;
        return 1;
    }

    if (last_computed < target) {
        recompute_coef(target);

        a_cache[0] = 1;       /* base of the recurrence */

        for (idx_t i = last_computed + 1; i <= target; ++i) {
            seq_t sum = 0;
            for (idx_t k = 1; k <= i; ++k)
                sum += coef_cache[k] * a_cache[i - k];
            a_cache[i] = sum / (seq_t)i;   /* division is exact */
        }
        last_computed = target;
    }

    return a_cache[target];
}
