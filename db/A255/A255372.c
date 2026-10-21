/*
 * A255372  Number of strings of n decimal digits that contain at least one
 *          string of exactly 2 consecutive "0" digits.
 * https://oeis.org/A255372
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A255372: Number of strings of n decimal digits that contain at least one
   substring of exactly two consecutive “0” digits.
   Formula: a(0)=0, a(1)=0, a(2)=1,
            a(n)=9*(10^(n-3)-a(n-3)+∑_{i=2}^{n-1}a(i))   for n≥3.   */

#include <stdlib.h>
#include "../utils/pow10.h"

static seq_t *a_cache = NULL;          /* array of a(i) values */
static seq_t *pref_cache = NULL;      /* prefix sums: pref[i] = Σ_{k=0}^{i} a(k) */
static size_t cache_cap = 0;           /* allocated capacity for the arrays */
static idx_t max_cached = 0;           /* highest index that has been computed */
static int initialized = 0;            /* flag indicating that the cache exists */

/* Ensure that values up to 'n' are computed and stored in the static arrays. */
static void ensure_cache_n(idx_t n)
{
    if (!initialized) {
        /* Allocate an initial capacity sufficient for indices 0..2. */
        cache_cap = 3;
        a_cache     = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        pref_cache  = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        /* Initial conditions: a(0)=0, a(1)=0, a(2)=1 */
        a_cache[0] = (seq_t)0;
        pref_cache[0] = a_cache[0];
        a_cache[1] = (seq_t)0;
        pref_cache[1] = pref_cache[0] + a_cache[1];
        a_cache[2] = (seq_t)1;
        pref_cache[2] = pref_cache[1] + a_cache[2];
        max_cached  = 2;
        initialized = 1;
    }

    if (n <= max_cached) return;

    /* Grow the allocated capacity if necessary. */
    while (cache_cap <= n) {
        size_t new_cap = cache_cap * 2;
        a_cache = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        pref_cache = (seq_t *)realloc(pref_cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Compute new values from max_cached+1 up to n. */
    for (idx_t i = max_cached + 1; i <= n; ++i) {
        if (i >= 3) {
            seq_t term = (seq_t)9 *
                         (pow10(i - 3) - a_cache[i - 3] + pref_cache[i - 1]);
            a_cache[i] = term;
            pref_cache[i] = pref_cache[i - 1] + term;
        }
    }

    max_cached = n;
}

/* External function required by the problem statement. */
seq_t a(idx_t n)
{
    ensure_cache_n(n);
    return a_cache[n];
}
