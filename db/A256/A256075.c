/*
 * A256075  Non-palindromic balanced numbers (in base 10).
 * https://oeis.org/A256075
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2213, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   balanced_ull, is_palindrome_ull
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Non‑palindromic balanced numbers (base 10).
   The function generates the sequence by brute‑force scanning from
   1000 upwards, testing each number for balancedness and
   non‑palindrome property. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/balanced_ull.h"
#include "../utils/is_palindrome_ull.h"

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Cache of already computed terms. */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static unsigned long long current = 999;   /* last tested candidate */

    if (cache_size < (size_t)n) {
        /* Ensure cache capacity. */
        size_t new_size = (size_t)n;
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache)
            return (seq_t)0;   /* graceful failure on allocation error */
        cache = new_cache;
    }

    while (cache_size < (size_t)n) {
        ++current;
        if (!balanced_ull(current))
            continue;
        if (is_palindrome_ull(current))
            continue;   /* skip palindromic numbers */
        cache[cache_size++] = (seq_t)current;
    }

    return cache[n - 1];
}
