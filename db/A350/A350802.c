/*
 * A350802  a(n) is the sum of the numbers k < n such that a(k) AND n = a(k)
 *          (where AND denotes the bitwise AND operator).
 * https://oeis.org/A350802
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(0) = 0; for n > 0, a(n) = Σ{k < n | (a(k) & n) == a(k)}
 * Range:     u8 n <= 30, u16 n <= 750, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation memoises values so that calls may arrive in any order. */

#include <stddef.h>   /* size_t, NULL */
#include <stdlib.h>   /* malloc, realloc, abort */

static seq_t *cache      = NULL;   /* stores a(0)...a(k) */
static idx_t  cache_len  = 0;       /* number of entries currently valid */
static size_t cache_cap  = 0;       /* allocated capacity */

/* Return a(n).  Works for any seq_t (fixed‑width or big integer).          */
seq_t a(idx_t n)
{
    /* Initialise the cache on first use.  a(0) is the only pre‑computed value. */
    if (cache_len == 0) {
        cache_cap = 8;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)0;      /* a(0) = 0 */
        cache_len = 1;            /* only a(0) stored so far */
    }

    /* Compute missing terms up to n (if any).                                   */
    if (n >= cache_len) {
        /* Grow the array if necessary.                                         */
        if ((size_t)n >= cache_cap) {
            size_t new_cap = cache_cap;
            while (new_cap <= (size_t)n) new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Compute a(m) for each new m.                                        */
        for (idx_t m = cache_len; m <= n; ++m) {
            seq_t sum = (seq_t)0;
            seq_t m_val = (seq_t)m;
            for (idx_t k = 0; k < m; ++k) {
                if ((cache[k] & m_val) == cache[k]) {
                    sum += (seq_t)k;
                }
            }
            cache[m] = sum;
        }
        cache_len = n + 1;   /* we now have a(0)...a(n) */
    }

    return cache[n];
}
