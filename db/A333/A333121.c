/*
 * A333121  a(1) = 1; a(n+1) = Product_{d|n} (1 + a(d)).
 * https://oeis.org/A333121
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; a(n+1) = ∏_{d|n} (1 + a(d))
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 10, u64 n <= 14, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;    /* 1‑based array of previously computed values */
static size_t cache_cap = 0;   /* allocated capacity (in seq_t elements) */
static idx_t cache_sz = 0;     /* highest index for which a(k) is known */

/* Compute the value of the OEIS sequence A333121 at position n. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined, but guard against accidental use */

    /* Ensure the base case a(1)=1 is available. */
    if (cache_sz == 0) {
        /* Allocate space for index 1.  Allocate at least 2 to avoid zero deallocation. */
        size_t cap = 2;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        cache_cap = cap;
        cache[1] = (seq_t)1;
        cache_sz = 1;
    }

    /* Grow the cache if necessary. */
    if (cache_sz < n) {
        size_t new_cap = (size_t)n + 1;   /* indices 0..n inclusive, 0 unused */
        if (new_cap > cache_cap) {
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) exit(1);            /* allocation failed – abort */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Compute values for indices cache_sz+1 .. n. */
        for (idx_t i = cache_sz + 1; i <= n; ++i) {
            idx_t k = i - 1;               /* divisors of n = k */
            seq_t prod = (seq_t)1;

            for (idx_t d = 1; d * d <= k; ++d) {
                if (k % d == 0) {
                    prod *= ((seq_t)1 + cache[d]);

                    idx_t e = k / d;
                    if (e != d) {
                        prod *= ((seq_t)1 + cache[e]);
                    }
                }
            }
            cache[i] = prod;
        }
        cache_sz = n;
    }

    return cache[n];
}
