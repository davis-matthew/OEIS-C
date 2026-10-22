/*
 * A393692  Number of plane partitions of n with 4 parts.
 * https://oeis.org/A393692
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) - 2*a(n-5) + a(n-8) + a(n-9) - a(n-10)
 *            with a(n)=0 for n<4 and the terms from the numerator: a(4)=5,
 *            a(6)=3, a(7)=2. This follows from (1-q)(1-q²)(1-q³)(1-q⁴)·A(q) =
 *            2q⁷+3q⁶+5q⁴
 * Range:     u8 n <= 15, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;      /* dynamic array, index 0…max_n */
static idx_t cache_n = 0;        /* highest n that is already computed */

/* compute a(n) for n ≥ 4, for any order of calls */
seq_t a(idx_t n)
{
    if (n < 4) return 0;       /* values below the offset are undefined */

    /* ensure the cache can hold indices up to n */
    if (cache_n < n) {
        size_t new_size = (size_t)(n + 1);
        seq_t *new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
        if (!new_cache) return 0;  /* out‑of‑memory – return 0 as a safe fallback */

        /* preserve old values */
        if (cache) {
            memcpy(new_cache, cache, (cache_n + 1) * sizeof(seq_t));
            free(cache);
        } else {
            /* no previous cache – initialise all to 0 */
            memset(new_cache, 0, new_size * sizeof(seq_t));
        }
        cache = new_cache;

        /* compute new entries from cache_n+1 up to n */
        for (idx_t i = cache_n + 1; i <= n; ++i) {
            seq_t c = 0;
            if (i == 4) c = 5;
            else if (i == 6) c = 3;
            else if (i == 7) c = 2;

            seq_t val = 0;
            if (i >= 1) val += cache[i - 1];
            if (i >= 2) val += cache[i - 2];
            if (i >= 8) val += cache[i - 8];
            if (i >= 9) val += cache[i - 9];
            if (i >= 5) val -= cache[i - 5] * 2;
            if (i >= 10) val -= cache[i - 10];
            val += c;

            cache[i] = val;
        }
        cache_n = n;
    }
    return cache[n];
}
