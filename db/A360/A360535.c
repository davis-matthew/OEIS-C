/*
 * A360535  Analog of Rudin-Shapiro sum sequence A020986, based on counting
 *          patterns 00 instead of 11.
 * https://oeis.org/A360535
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{i=0..n} (-1)^e(i), where e(i) counts (possibly
 *            overlapping) occurrences of the pattern 00 in the binary
 *            representation of i
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Validated: partial, pass, 70 terms through n = 69 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

/* Count the occurrences of 00 in the binary representation of x.
   x is treated without leading zeros. */
static inline int count_zero_zero(idx_t x)
{
    if (x == 0)
        return 0;

    /* Find the most significant bit position. */
    int msb = 0;
    idx_t t = x;
    while (t > 1) {
        t >>= 1;
        msb++;
    }

    int cnt = 0;
    int prev = 1;                    /* a leading 1 protects against counting leading zeros. */

    for (int bit = msb; bit >= 0; --bit) {
        int cur = (int)((x >> bit) & 1U);
        if (prev == 0 && cur == 0)
            ++cnt;
        prev = cur;
    }
    return cnt;
}

/* Cache of computed prefix sums. */
static seq_t *cache = NULL;
static size_t cache_cap = 0;

/* Compute a(n).  Results are memoised so subsequent calls are fast. */
seq_t a(idx_t n)
{
    if (n >= cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *new_cache = mem_new(seq_t, new_cap);
        if (!new_cache)
            abort();          /* Out of memory – terminate. */

        /* Copy old values if any. */
        if (cache) {
            for (size_t i = 0; i < cache_cap; ++i)
                new_cache[i] = cache[i];
            mem_free(cache);
        }

        /* Initialise from previous capacity to the new bound. */
        for (size_t i = cache_cap; i < new_cap; ++i) {
            if (i == 0) {
                /* e(0) = 0 ⇒ (-1)^0 = +1. */
                new_cache[0] = (seq_t)1;
            } else {
                int e = count_zero_zero((idx_t)i);
                seq_t prev = new_cache[i - 1];
                if ((e & 1) == 0)
                    new_cache[i] = prev + (seq_t)1;
                else
                    new_cache[i] = prev - (seq_t)1;
            }
        }

        cache = new_cache;
        cache_cap = new_cap;
    }

    return cache[(size_t)n];
}
