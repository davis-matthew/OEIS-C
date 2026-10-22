/*
 * A344347  Numbers k such that sigma(k)^2 is divisible by k-1.
 * https://oeis.org/A344347
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 30, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers k such that sigma(k)^2 is divisible by k-1
   It is computed by an incremental search and cached for later calls */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/sigma_of.h"

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
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
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
