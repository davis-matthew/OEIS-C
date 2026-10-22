/*
 * A371213  Number of ordered factorizations of n into factors > 5.
 * https://oeis.org/A371213
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5039, u16 n <= 8977, u32 n <= 8977, u64 n <= 8977,
 *            u128 n <= 8977, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A371213: Number of ordered factorizations of n into factors > 5
   Formula: a(1) = 1; for n > 1,
   a(n) = Σ_{d|n, n/d > 5} a(d)                           */
#include <stdlib.h>   /* malloc, free, abort */

static seq_t *seq_cache = NULL;   /* cache[0] unused, cache[1] = 1      */
static idx_t cache_cap = 0;       /* number of allocated elements       */

/* Ensure that seq_cache contains values up to index n (inclusive). */
static void ensure_cache(idx_t n) {
    if (n + 1 <= cache_cap) return;           /* already computed            */

    idx_t old_cap = cache_cap;
    idx_t new_cap = n + 1;                    /* indices 0..n inclusive      */
    seq_t *new_arr = (seq_t*)malloc(new_cap * sizeof(seq_t));
    if (!new_arr) abort();                   /* allocation failure          */

    /* initialise new array with zero and copy old values */
    for (idx_t i = 0; i < new_cap; ++i) new_arr[i] = 0;
    for (idx_t i = 0; i < old_cap; ++i) new_arr[i] = seq_cache[i];

    /* free old buffer */
    free(seq_cache);
    seq_cache = new_arr;
    cache_cap = new_cap;

    /* initialise base cases */
    if (cache_cap > 1) seq_cache[1] = 1;       /* a(1) = 1 */

    /* compute missing values from old_cap up to n */
    idx_t start = old_cap;
    if (start <= 1) start = 2;                /* skip 0 and 1, already set   */
    for (idx_t k = start; k < cache_cap; ++k) {
        seq_t sum = 0;
        /* iterate over all divisors d of k */
        for (idx_t d = 1; d * d <= k; ++d) {
            if (k % d == 0) {
                idx_t d1 = d;
                idx_t d2 = k / d;
                /* contribution from divisor d1 */
                if (k / d1 > 5) sum += seq_cache[d1];
                /* contribution from the paired divisor d2,
                   avoid double‑counting when d1 == d2          */
                if (d1 != d2 && k / d2 > 5) sum += seq_cache[d2];
            }
        }
        seq_cache[k] = sum;
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 0;            /* safety guard, offset is 1      */
    ensure_cache(n);
    return seq_cache[n];
}
