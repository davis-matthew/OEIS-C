/*
 * A283882  Relative of Hofstadter Q-sequence: a(n) = max(0, n+67) for n <= 0;
 *          a(n) = a(n-a(n-1)) + a(n-a(n-2)) for n > 0.
 * https://oeis.org/A283882
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n <= 0)?max(0,n+67): a(n-a(n-1)) + a(n-a(n-2)) (offset
 *            1)
 * Range:     u8 n <= 39, u16 n <= 322, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Global cache for computed a(k) with k >= 0.
   seq_cache[0] holds a(0) = 67. */
static seq_t *seq_cache = NULL;
static idx_t cache_cap   = 0;   /* allocated size (>= required index + 1) */
static idx_t last_comp   = 0;   /* highest index computed and stored in seq_cache */

/* Helper to compute a(i - prev) where:
   - 'i'   : current index (>=1)
   - 'prev': value a(i-1) or a(i-2).
   This function handles both positive (cache lookup) and non‑positive arguments. */
static inline seq_t term(idx_t i, seq_t prev)
{
    if (prev > (seq_t)i) {
        /* i - prev <= 0 → a(i-prev) = max(0, (i-prev)+67) */
        seq_t diff = prev - (seq_t)i;          /* diff = prev - i >= 1          */
        if (diff >= 67)                        /* |i-prev| >= 67 → result 0   */
            return 0;
        return (seq_t)67 - diff;              /* 67 - (prev-i) */
    } else {
        /* i - prev >= 0 → use cached value */
        idx_t k = (idx_t)(i - prev);
        return seq_cache[k];
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)67;   /* defined but never requested by callers */

    /* Ensure cache can address indices 0…n. */
    if (cache_cap <= n) {
        idx_t new_cap = n + 1;
        if (seq_cache == NULL) {
            seq_cache = (seq_t *)calloc((size_t)new_cap, sizeof(seq_t));
        } else {
            seq_cache = (seq_t *)realloc(seq_cache, (size_t)new_cap * sizeof(seq_t));
        }
        /* Out of memory is unlikely; if it occurs, return 0. */
        if (seq_cache == NULL) return 0;
        seq_cache[0] = (seq_t)67;   /* a(0) */
        cache_cap   = new_cap;
        last_comp   = 0;
    }

    if (n <= last_comp) return seq_cache[n];

    /* Compute missing terms up to n in order. */
    for (idx_t i = last_comp + 1; i <= n; ++i) {
        seq_t p1 = seq_cache[i - 1];                     /* a(i-1) */
        seq_t p2 = (i >= 2) ? seq_cache[i - 2] : (seq_t)66; /* a(i-2), a(-1)=66 */
        seq_cache[i] = term(i, p1) + term(i, p2);
    }
    last_comp = n;
    return seq_cache[n];
}
