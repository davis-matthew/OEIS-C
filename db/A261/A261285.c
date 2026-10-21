/*
 * A261285  Number of (n+2) X (1+2) 0..1 arrays with each 3 X 3 subblock
 *          having clockwise perimeter pattern 00000001 00010101 or 01010101.
 * https://oeis.org/A261285
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 14, u32 n <= 32, u64 n <= 69, u128 n <= 144,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-2) + 2*a(n-3) + 4*a(n-4) + 3*a(n-6)
   - 8*a(n-7) + 9*a(n-8) - 6*a(n-9) - 4*a(n-10) - 4*a(n-11) - 4*a(n-12)
   Initial values a(1) … a(13) are 36, 50, 96, 166, 307, 540, 1015,
   1786, 3304, 5862, 10877, 19316, 35609.  All computations are done for
   any seq_t (fixed‑size or arbitrary precision).  The sequence is memoised
   in a static array that is zeroed when expanded. */

#include <stdlib.h>
#include <stdint.h>

extern seq_t a(idx_t n); /* forward declaration for the linker */

static seq_t *cache      = NULL;   /* cache[0] unused; cache[i] = a(i) */
static size_t cache_cap  = 0;      /* number of elements currently allocated */

/* Ensure that CACHE has room for indices up to N (inclusive).  New elements
   are zeroed so that the test (cache[i] != 0) correctly detects uninitialised
   entries. */
static void
ensure_cache(idx_t n)
{
    if ((size_t)n >= cache_cap) {
        size_t new_cap = (size_t)n + 1;            /* +1 for direct indexing */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            abort();
        /* Zero initialise the newly allocated part of the array */
        if (tmp != cache) {
            /* realloc may move the block, keep old data */
        }
        for (size_t j = cache_cap; j < new_cap; ++j)
            tmp[j] = 0;
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Base values for n = 1 … 13; index 0 is unused. */
static const uint64_t base_raw[14] = {
    0ULL,          /* unused */
    36ULL, 50ULL, 96ULL, 166ULL, 307ULL,
    540ULL, 1015ULL, 1786ULL, 3304ULL,
    5862ULL, 10877ULL, 19316ULL, 35609ULL
};

seq_t
a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;          /* undefined for n < 1, return 0 */

    ensure_cache(n);

    for (idx_t i = 1; i <= n; ++i) {
        /* if already computed, skip */
        if (cache[(size_t)i] != (seq_t)0)
            continue;

        if (i <= 13ULL) {
            cache[(size_t)i] = (seq_t)base_raw[i];
        } else {
            /* recurrence for i > 13 */
            seq_t pos = cache[(size_t)(i - 2)];
            pos += 2 * cache[(size_t)(i - 3)];
            pos += 4 * cache[(size_t)(i - 4)];
            pos += 3 * cache[(size_t)(i - 6)];
            pos += 9 * cache[(size_t)(i - 8)];

            seq_t neg = 8 * cache[(size_t)(i - 7)];
            neg += 6 * cache[(size_t)(i - 9)];
            neg += 4 * cache[(size_t)(i - 10)];
            neg += 4 * cache[(size_t)(i - 11)];
            neg += 4 * cache[(size_t)(i - 12)];

            cache[(size_t)i] = pos - neg;
        }
    }

    return cache[(size_t)n];
}
