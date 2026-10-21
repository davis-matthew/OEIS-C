/*
 * A259460  From higher-order arithmetic progressions.
 * https://oeis.org/A259460
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 4, u64 n <= 9, u128 n <= 16,
 *            big any n
 * Helpers:   gcd64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A259460
   Recurrence (valid for n >= 1):
       a(n) = [(n+3)^2 * (n+4) * (3*n+8) / (2*n * (3*n+5))] * a(n-1)
   Initial value: a(0) = 4
*/

#include <stddef.h>   /* for NULL */
#include <stdlib.h>   /* for realloc */
#include "../utils/gcd64.h"

/* GCD for unsigned 64‑bit values */

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;   /* allocated capacity (in elements) */
    static idx_t cache_len = 0;   /* number of computed entries (max index + 1) */

    /* Ensure the cache can hold entries up to index `n`. */
    if (n >= cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap : 1;
        while (new_cap <= n) new_cap <<= 1;
        /* Use the provided seq_realloc if available, otherwise fall back to realloc. */
        #ifdef seq_realloc
        seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
        #else
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        #endif
        cache = tmp;
        cache_cap = new_cap;
        /* Initialise new slots to zero (not strictly necessary, but safe). */
        for (idx_t i = cache_len; i < cache_cap; ++i)
            cache[i] = (seq_t)0;
    }

    /* Initialise the cache with the first value a(0) = 4. */
    if (cache_len == 0) {
        cache[0] = (seq_t)4;
        cache_len = 1;
    }

    /* Return cached value if already computed. */
    if (n < cache_len)
        return cache[n];

    /* Compute terms incrementally using the recurrence. */
    for (idx_t k = cache_len; k <= n; ++k) {
        seq_t val = cache[k-1];   /* a(k-1) */

        /* Prepare numerator and denominator factors (64‑bit). */
        uint64_t nf[4];
        uint64_t nd[2];
        nf[0] = (k + 3);               /* (k+3) */
        nf[1] = (k + 3);               /* (k+3) again */
        nf[2] = (k + 4);               /* (k+4) */
        nf[3] = 3ULL * k + 8;          /* (3*k+8) */

        nd[0] = 2ULL * k;              /* 2*k */
        nd[1] = 3ULL * k + 5;          /* (3*k+5) */

        /* Reduce numerator and denominator factors by gcds to keep numbers small. */
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 2; ++j) {
                if (nd[j] == 1ULL) continue;
                uint64_t g = gcd64(nf[i], nd[j]);
                if (g > 1ULL) {
                    nf[i] /= g;
                    nd[j] /= g;
                }
            }
        }

        /* Multiply remaining numerator factors into val. */
        for (int i = 0; i < 4; ++i)
            val *= (seq_t)nf[i];

        /* Divide by remaining denominator factors. */
        for (int j = 0; j < 2; ++j)
            if (nd[j] > 1ULL)
                val /= (seq_t)nd[j];

        cache[k] = val;
    }

    cache_len = n + 1;
    return cache[n];
}
