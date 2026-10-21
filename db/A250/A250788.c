/*
 * A250788  Number of (6+1) X (n+1) 0..1 arrays with nondecreasing
 *          x(i,j)+x(i,j-1) in the i direction and nondecreasing absolute
 *          value of x(i,j)-x(i-1,j) in the j direction.
 * https://oeis.org/A250788
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n‑1) - 18*a(n‑2) + 17*a(n‑3) + 8*a(n‑4) - 29*a(n‑5) +
 *            18*a(n‑6) + 3*a(n‑7) - 7*a(n‑8) + 2*a(n‑9) with a(1) = 288, a(2)
 *            = 796, a(3) = 2010, a(4) = 5028, a(5) = 12036, a(6) = 27986,
 *            a(7) = 63184, a(8) = 139436, a(9) = 301786 for n ≥ 10
 * Range:     u8 none, u16 n <= 7, u32 n <= 22, u64 n <= 54, u128 n <= 118,
 *            big any n
 * Helpers:   add_mult
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/add_mult.h"

/* Store the first nine values.  They are needed even for the smallest n. */
static const seq_t init_vals[9] = {
    (seq_t)288ULL, (seq_t)796ULL, (seq_t)2010ULL, (seq_t)5028ULL,
    (seq_t)12036ULL, (seq_t)27986ULL, (seq_t)63184ULL,
    (seq_t)139436ULL, (seq_t)301786ULL
};

/* Dynamic cache: cache[i] holds a(i); cache[0] unused. */
static seq_t *cache      = NULL;   /* pointer to array of seq_t */
static size_t cache_cap  = 0;      /* allocated size (elements)   */
static size_t cache_len  = 0;      /* highest n whose value is stored */

/* Ensure cache can hold values up to (inclusive) max_n. */
static void ensure_cache_capacity(idx_t max_n) {
    size_t need  = (size_t)max_n + 1;   /* include index 0          */
    if (cache == NULL) {
        cache     = (seq_t*)calloc(need, sizeof(seq_t));
        cache_cap = need;
        cache_len = 0;
    }
    if (need > cache_cap) {
        seq_t *new_cache = (seq_t*)realloc(cache, need * sizeof(seq_t));
        /* Zero new elements that will be filled later. */
        for (size_t i = cache_cap; i < need; ++i) new_cache[i] = (seq_t)0;
        cache     = new_cache;
        cache_cap = need;
    }
}

/* Fill cache[1..9] with the hard‑coded values if not yet done. */
static void init_cache_initial_values(void) {
    if (cache_len >= 9) return;
    if (cache_cap < 10) { /* just in case, allocate at least 10 slots */
        ensure_cache_capacity(9);
    }
    for (size_t i = 1; i <= 9; ++i) cache[i] = init_vals[i-1];
    cache_len = 9;
}

/* Main sequence function */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;                 /* undefined, guard */
    /* Ensure we have room for the first nine values even if n < 9. */
    ensure_cache_capacity((idx_t)((n > 9) ? n : 9));
    init_cache_initial_values();

    if ((size_t)n <= cache_len) return cache[n];

    /* Compute new values up to n using the recurrence. */
    for (size_t k = cache_len + 1; k <= (size_t)n; ++k) {
        seq_t pos = (seq_t)0, neg = (seq_t)0;

        /* Positive terms */
        add_mult(&pos, &cache[k-1], 7);
        add_mult(&pos, &cache[k-3], 17);
        add_mult(&pos, &cache[k-4], 8);
        add_mult(&pos, &cache[k-6], 18);
        add_mult(&pos, &cache[k-7], 3);
        add_mult(&pos, &cache[k-9], 2);

        /* Negative terms */
        add_mult(&neg, &cache[k-2], 18);
        add_mult(&neg, &cache[k-5], 29);
        add_mult(&neg, &cache[k-8], 7);

        cache[k] = pos - neg;   /* values are guaranteed non‑negative */
    }
    cache_len = (size_t)n;
    return cache[n];
}
