/*
 * A390031  Number of ordered factorizations of 2*n into factors > 1.
 * https://oeis.org/A390031
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of ordered factorizations of 2*n into factors > 1. */
#include <stdlib.h>

/* Cached values of f(m) = number of ordered factorizations of m (m≥1). */
static seq_t *f_cache = NULL;    /* array index == m */
static size_t f_cache_cap = 0;   /* allocated size */
static idx_t f_cache_max = 0;    /* maximum m computed so far */

/* Extend cache capacity to hold at least 'need' indices (0 … need). */
static void
ensure_f_cache(idx_t need)
{
    if ((idx_t)f_cache_cap > need)
        return;
    size_t new_cap = f_cache_cap ? f_cache_cap : 4;
    while ((idx_t)new_cap <= need)
        new_cap <<= 1;
    seq_t *new_cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
    if (!new_cache)
        abort();
    /* Copy existing data */
    for (size_t i = 0; i < f_cache_cap; ++i)
        new_cache[i] = f_cache[i];
    /* Zero new entries */
    for (size_t i = f_cache_cap; i < new_cap; ++i)
        new_cache[i] = 0;
    free(f_cache);
    f_cache = new_cache;
    f_cache_cap = new_cap;
}

/* Main function required by the build. */
seq_t a(idx_t n)
{
    if (n == 0)                 /* a(0) = 0 by definition */
        return (seq_t)0;

    idx_t m = 2 * n;            /* target value to compute: f(2*n) */

    /* Make sure we can address up to index m */
    ensure_f_cache(m);

    /* Initialise base case f(1) if not already done */
    if (f_cache_max < 1) {
        f_cache[1] = (seq_t)1;
        f_cache_max = 1;
    }

    /* Compute f(i) for all new m > f_cache_max up to m */
    for (idx_t i = f_cache_max + 1; i <= m; ++i) {
        if (i == 1) {               /* base case handled above */
            f_cache[i] = (seq_t)1;
            continue;
        }
        seq_t sum = (seq_t)1;        /* accounts for divisor d = i (factor [i]) */
        /* Enumerate divisors d of i with 2 ≤ d ≤ sqrt(i) */
        for (idx_t d = 2; d * d <= i; ++d) {
            if (i % d == 0) {
                idx_t q = i / d;
                sum += f_cache[q];        /* first factor = d */
                if (q != d)               /* first factor = q (the paired divisor) */
                    sum += f_cache[d];
            }
        }
        f_cache[i] = sum;
    }
    f_cache_max = m;
    return f_cache[m];
}
