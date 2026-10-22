/*
 * A397213  G.f. A(x) satisfies (1-2*x)*A(x) + A(x^4) = x.
 * https://oeis.org/A397213
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; for n >= 2, a(n) = 2*a(n-1) - (n%4==0 ? a(n/4) : 0)
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements this recurrence with memoisation.          */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>   /* for uint64_t if needed */

static seq_t *cache = NULL;      /* stores a(1) .. a(cache_len) */
static size_t cache_len = 0;     /* number of values currently stored */
static size_t cache_cap = 0;     /* allocated capacity */

/* Grow cache to accommodate at least 'need' elements (1‑based). */
static void ensure_capacity(size_t need)
{
    if (cache_cap < need) {
        size_t new_cap = cache_cap ? cache_cap : 1;
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                      /* undefined below offset */

    size_t idx = (size_t)n;                    /* 1‑based index in C range */

    /* Initialise base case if needed */
    if (cache_len == 0) {
        ensure_capacity(1);
        cache[0] = 1;                          /* a(1) */
        cache_len = 1;
    }

    /* If the value is already cached, return it. */
    if (idx <= cache_len) return cache[idx - 1];

    /* Ensure we have room for all required values. */
    ensure_capacity(idx);

    /* Compute missing entries from cache_len+1 up to idx. */
    for (size_t i = cache_len + 1; i <= idx; ++i) {
        seq_t val = cache[i - 2] * 2;           /* 2*a(i-1) */

        if ((i & 3ULL) == 0) {                  /* i divisible by 4 */
            size_t q = i >> 2;                  /* i/4 */
            val -= cache[q - 1];                /* subtract a(i/4) */
        }

        cache[i - 1] = val;
    }

    cache_len = idx;
    return cache[idx - 1];
}
