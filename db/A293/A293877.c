/*
 * A293877  Numbers having '17' as substring of their digits / decimal
 *          expansion.
 * https://oeis.org/A293877
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 2989, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   contains_substring_17
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth integer that contains the decimal substring "17". */
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/contains_substring_17.h"

/* Ensure a dynamic array has room for at least `needed` elements. */
static void ensure_capacity(size_t *cap, seq_t **arr, size_t needed)
{
    if (*cap < needed)
    {
        size_t new_cap = (*cap == 0) ? 16 : (*cap << 1);
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
        if (!tmp) abort();                 /* simple error handling */
        *arr   = tmp;
        *cap   = new_cap;
    }
}

/* Cache of already computed terms. */
static seq_t  *seq_cache = NULL;
static size_t  cache_cap = 0;
static idx_t   cache_size = 0;

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* undefined – never called with n==0 */

    /* Grow cache up to `n` terms. */
    while (cache_size < n)
    {
        seq_t candidate;
        if (cache_size == 0)
            candidate = 17;
        else
            candidate = seq_cache[cache_size - 1] + 1;

        /* Find the next number containing "17". */
        while (!contains_substring_17(candidate))
            candidate += 1;

        /* Append to cache. */
        ensure_capacity(&cache_cap, &seq_cache, cache_size + 1);
        seq_cache[cache_size++] = candidate;
    }

    return seq_cache[n - 1];
}
