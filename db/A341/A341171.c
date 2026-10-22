/*
 * A341171  Numbers that when divided by the sum of their digits leave 4 as
 *          remainder.
 * https://oeis.org/A341171
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 2385, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A341171: numbers N such that N ≡ 4 (mod sum_of_digits(N)) */
#include <stdlib.h>
#include "../utils/digit_sum.h"

/* Cache to store already found terms. */
static seq_t *cache = NULL;   /* dynamic array of terms */
static size_t cache_len = 0;  /* number of terms stored */
static size_t cache_cap = 0;  /* allocated capacity of cache */
static seq_t next_candidate = 1; /* smallest integer to test next */

/* Ensure the cache has at least 'needed' capacity. */
static void cache_ensure_cap(size_t needed)
{
    if (needed <= cache_cap)
        return;

    size_t new_cap = cache_cap ? (cache_cap << 1) : 64;
    while (new_cap < needed)
        new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();              /* terminate on out‑of‑memory */
    cache = tmp;
    cache_cap = new_cap;
}

/* Return the n-th element of the sequence (n ≥ 1). */
seq_t a(idx_t n)
{
    /* We ignore n == 0 – it is never requested in the tests. */
    if (n == 0)
        return 0;

    /* Generate terms until we have at least n of them. */
    while (cache_len < (size_t)n)
    {
        seq_t s = digit_sum(next_candidate);
        if (s != 0 && (next_candidate % s) == 4)
        {
            cache_ensure_cap(cache_len + 1);
            cache[cache_len++] = next_candidate;
        }
        next_candidate++;
    }

    return cache[(size_t)n - 1];
}
