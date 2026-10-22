/*
 * A320615  Number of ordered set partitions of [n] where k = two is minimal
 *          such that for each block b the smallest integer interval
 *          containing b has at most k elements.
 * https://oeis.org/A320615
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = A240172(n) – n! = Σ_{t=1}^{⎣n/2⎦} C(n−t, t) · (n−t)! (n ≥
 *            2)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* cache for factorials */
static seq_t *fact_cache = NULL;
static size_t fact_cap   = 0;

/* Ensure factorial cache is up to and including n */
static void ensure_factorial_cache(idx_t n)
{
    if (n + 1 <= fact_cap)
        return;

    size_t new_cap = (size_t)(n + 1);
    seq_t *new_cache = (seq_t *)malloc(sizeof(seq_t) * new_cap);
    if (!new_cache)
        return; /* allocation failed – caller will get 0 */

    /* copy existing values */
    if (fact_cache) {
        for (size_t i = 0; i < fact_cap; ++i)
            new_cache[i] = fact_cache[i];
    }

    /* if cache was empty, initialise factorial 0! */
    if (!fact_cache)
        new_cache[0] = (seq_t)1;

    /* compute remaining factorials */
    idx_t start = (idx_t)(fact_cache ? fact_cap : 1);
    for (idx_t i = start; i <= n; ++i)
        new_cache[i] = new_cache[i - 1] * (seq_t)i;

    free(fact_cache);
    fact_cache = new_cache;
    fact_cap   = new_cap;
}

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;           /* values below the offset are not used */

    ensure_factorial_cache(n);
    seq_t total = (seq_t)0;

    idx_t lim = n / 2;
    for (idx_t t = 1; t <= lim; ++t)
    {
        idx_t k = n - t;                      /* number of blocks */
        seq_t binom = (seq_t)1;
        /* C(k, t) with t ≤ k/2 (here k = n‑t and t ≤ floor(n/2)) */
        for (idx_t i = 1; i <= t; ++i)
            binom = (binom * ((seq_t)(k - t + i))) / (seq_t)i;

        seq_t term = binom * fact_cache[k];
        total += term;
    }
    return total;
}
