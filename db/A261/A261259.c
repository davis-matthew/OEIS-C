/*
 * A261259  Number of (n+2) X (2+2) 0..1 arrays with each 3 X 3 subblock
 *          having clockwise perimeter pattern 00010101 00100101 or 01010101.
 * https://oeis.org/A261259
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 2*a(n-3) + a(n-4) + 2*a(n-5) for n>6 Initial
 *            values: a(1)=41, a(2)=52, a(3)=110, a(4)=232, a(5)=418, a(6)=758
 *            This implementation uses memoised recurrence to obtain any a(n)
 *            (n≥1)
 * Range:     u8 n <= 4, u16 n <= 13, u32 n <= 30, u64 n <= 65, u128 n <= 136,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;          /* memoisation buffer, 1‑based indices */
static size_t cache_cap = 0;         /* current allocated capacity (≥ n+1) */
static size_t cache_len = 0;         /* highest index that has been computed */

/* ensures cache has room for indices up to n (inclusive) */
static void _ensure_cache(idx_t n)
{
    idx_t needed = n + 1;            /* because we ignore index 0 */
    if (cache_cap < (size_t)needed) {
        size_t new_cap = (size_t)needed;
        seq_t *new_cache;
        if (cache) {
            new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        } else {
            new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        }
        if (!new_cache) {           /* allocation failed – abort */
            abort();
        }
        cache = new_cache;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) {
        /* behaviour for n < 1 is undefined; return 0 as a safe sentinel */
        return (seq_t)0;
    }

    if (cache_len >= (size_t)n) {   /* already computed */
        return cache[n];
    }

    _ensure_cache(n);

    /* initialise base values if this is the first call */
    if (cache_len == 0) {
        if (cache_cap <= 6) {
            /* ensure capacity for indices 1..6, safe because cache_cap ≥ n+1 ≥ 7 */
            _ensure_cache(6);
        }
        cache[1] = (seq_t)41;
        cache[2] = (seq_t)52;
        cache[3] = (seq_t)110;
        cache[4] = (seq_t)232;
        cache[5] = (seq_t)418;
        cache[6] = (seq_t)758;
        cache_len = 6;
    }

    /* compute values up to n */
    for (idx_t i = (idx_t)(cache_len + 1); i <= n; ++i) {
        seq_t val;
        if (i <= 6) {
            /* base values already set; skip calculation */
            continue;
        }
        /* recurrence: a(i) = a(i-1) + 2*a(i-3) + a(i-4) + 2*a(i-5) */
        val = cache[i - 1] + (seq_t)2 * cache[i - 3]
                + cache[i - 4] + (seq_t)2 * cache[i - 5];
        cache[i] = val;
    }

    cache_len = (size_t)n;
    return cache[n];
}
