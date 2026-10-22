/*
 * A366210  Numbers whose median of the digits is equal to 4.
 * https://oeis.org/A366210
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   median_is_four
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A366210.
   The function scans natural numbers, keeps those whose median of decimal
   digits equals 4, and stores them in a growing cache.  Subsequent
   calls reuse the cache, so any index is answered in amortised O(1)
   time.  The implementation is fully portable to C11 and C++17. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/median_is_four.h"

/* Cache for already computed terms. */
static seq_t *cache             = NULL;   /* array of terms */
static size_t cache_len         = 0;      /* number of terms stored */
static size_t cache_cap         = 0;      /* current capacity of cache */
static seq_t next_candidate     = (seq_t)1;/* next integer to test */

/* Return the n‑th term of the sequence, 1‑based indexing (a(1) is the first). */
seq_t a(idx_t n)
{
    if (n == 0)                        /* undefined below the offset */
        return (seq_t)0;

    /* Initialise the cache on first call. */
    if (cache == NULL)
    {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* Generate terms until we reach the requested index. */
    while (cache_len < n)
    {
        if (median_is_four(next_candidate))
            cache[cache_len++] = next_candidate;
        ++next_candidate;
        if (cache_len == cache_cap)          /* grow cache */
        {
            cache_cap <<= 1;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }
    }

    return cache[n - 1];
}
