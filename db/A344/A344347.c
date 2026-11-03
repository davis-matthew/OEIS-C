/*
 * A344347  Numbers k such that sigma(k)^2 is divisible by k-1.
 * https://oeis.org/A344347
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 30, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Validated: partial, pass, 45 terms through n = 45 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers k such that sigma(k)^2 is divisible by k-1
   It is computed by an incremental search and cached for later calls */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

/* Cache for the sequence values */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t next_candidate = 2;   /* start from the smallest k to test */

/* Ensure the cache can hold at least 'needed' terms. */
static void grow_cache(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
    if (!tmp)
        abort();                 /* fatal: cannot allocate more memory */
    cache = tmp;
    cache_cap = new_cap;
}

/* Return the n-th element of A344347 (1‑based index). */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined for n = 0, return 0 as safety */

    while (cache_len < n)
    {
        seq_t sigma = sigma_of(next_candidate);
        seq_t sq = sigma * sigma;
        seq_t d   = (seq_t)(next_candidate - 1);
        if ((sq % d) == 0)
        {
            grow_cache(cache_len + 1);
            cache[cache_len++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }
    return cache[n - 1];
}
