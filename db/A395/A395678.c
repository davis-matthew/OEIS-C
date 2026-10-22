/*
 * A395678  Numbers which are divisible by the sum of their base-9 digits.
 * https://oeis.org/A395678
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 90, u16 n <= 9473, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   base9_digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search algorithm: numbers divisible by the sum of their base‑9 digits (9‑Harshad numbers) */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/base9_digit_sum.h"

/* Static cache for memoisation */
static seq_t *cache = NULL;          /* stored sequence values */
static size_t cache_size = 0;        /* number of values already computed */
static size_t cache_cap   = 0;        /* allocated capacity of the cache */
static seq_t next_candidate = 1;     /* next integer to test */

/* Make sure the cache can hold at least 'need' elements */
static void ensure_cache_capacity(size_t need)
{
    if (cache_cap >= need)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    if (new_cap < need)
        new_cap = need;
    /* realloc works in C and can be cast safely in C++ */
    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();
    cache = tmp;
    cache_cap = new_cap;
}

/* Append a value to the cache */
static void cache_append(seq_t val)
{
    ensure_cache_capacity(cache_size + 1);
    cache[cache_size++] = val;
}

/* Return the n-th 9‑Harshad number (a(1)=1) */
seq_t a(idx_t n)
{
    if (n == 0)             /* Undefined for n==0; return 0 as a safe default */
        return 0;

    /* Generate terms until the n‑th one is ready */
    while (cache_size < (size_t)n) {
        seq_t val = next_candidate;
        seq_t sum = base9_digit_sum(val);
        if (sum != 0 && (val % sum) == 0)
            cache_append(val);
        ++next_candidate;
    }

    return cache[n - 1];
}
