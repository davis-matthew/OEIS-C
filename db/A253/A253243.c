/*
 * A253243  Expansion of phi(-x^2) * psi(x^3) * chi(x^3) in powers of x where
 *          phi(), psi(), chi() are Ramanujan theta functions.
 * https://oeis.org/A253243
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 32, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A253243: Expansion of φ(-x²)ψ(x³)χ(x³)
 *  a(0) = 1 ,  a(1) = 0 ,  a(2) = -2 , …
 *  Generating function (period‑12 Euler transform)
 *  a(n) = (1/n) * Σ_{k=1..n} g[k] * a[n‑k]
 *  where g[k] = Σ_{d|k} d * b(d)  and b(d) repeats the pattern
 *  [0, -2, 2, -1, 0, -4, 0, -1, 2, -2, 0, -2].
 */

#include <stdint.h>
#include <stdlib.h>

/* Periodic sequence b(d) for d ≥ 1; index 0 → d = 1 */
static const int period[12] = {0, -2, 2, -1, 0, -4, 0, -1, 2, -2, 0, -2};

/* Cache for previously computed a(n) values */
static seq_t *a_cache = NULL;
static idx_t  a_cache_limit = 0;

/* Recompute a(0)…a(limit) and store in a_cache (replaces old cache) */
static void recompute_a(idx_t limit)
{
    size_t sz = (size_t)limit + 1;

    /* Allocate new arrays */
    seq_t *new_cache = (seq_t *)malloc(sz * sizeof(seq_t));
    seq_t *g         = (seq_t *)malloc(sz * sizeof(seq_t));

    /* Initialise g[] to 0 */
    for (size_t i = 0; i < sz; ++i) {
        g[i] = 0;
        new_cache[i] = 0;
    }

    /* Compute g[k] = Σ_{d|k} d * b(d)   (k ≥ 1) */
    for (size_t d = 1; d <= limit; ++d) {
        int bd   = period[(d - 1) % 12];
        seq_t term = (seq_t)d * (seq_t)bd;
        for (size_t m = d; m <= limit; m += d) {
            g[m] += term;
        }
    }

    /* a(0) = 1   (Euler transform starts with 1) */
    new_cache[0] = 1;

    /* Recurrence:
       a[n] = (1/n) * Σ_{k=1}^{n} g[k] * a[n-k]   (n ≥ 1)   */
    for (size_t n = 1; n <= limit; ++n) {
        seq_t sum = 0;
        for (size_t k = 1; k <= n; ++k) {
            sum += g[k] * new_cache[n - k];
        }
        new_cache[n] = sum / (seq_t)n;          /* division is exact */
    }

    /* Replace old cache */
    free(a_cache);
    free(g);
    a_cache = new_cache;
    a_cache_limit = limit;
}

/* Public entry point */
seq_t a(idx_t n)
{
    /* Initialise or extend cache as needed */
    if (!a_cache || n > a_cache_limit) {
        recompute_a((idx_t)n);
    }
    return a_cache[(size_t)n];
}
