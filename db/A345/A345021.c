/*
 * A345021  a(n) is the result of replacing 2's by 0's in the hereditary
 *          base-2 expansion of n.
 * https://oeis.org/A345021
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of set‐bit positions e of n such that a(e) == 0 *
 *            (a(0) = 0, a(1) = 1). * Thus a(n) can be computed recursively by
 *            summing over the binary * expansion of n. The result equals
 *            A342707(n,0)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Memoisation storage */
static seq_t *cache = NULL;          /* computed a(e) values                */
static bool *seen  = NULL;           /* whether cache[n] has a valid value  */
static size_t cache_cap = 0;         /* current capacity of cache arrays    */

/* Ensure that the memoisation arrays can hold indices up to `limit`. */
static void grow_cache(idx_t limit)
{
    if (limit + 1 <= cache_cap) return;

    size_t new_cap = (size_t)limit + 1;
    seq_t *new_cache = (seq_t *)calloc(new_cap, sizeof(seq_t));
    bool   *new_seen  = (bool   *)calloc(new_cap, sizeof(bool));

    /* Copy old data if any */
    if (cache && seen) {
        memcpy(new_cache, cache, cache_cap * sizeof(seq_t));
        memcpy(new_seen,  seen,  cache_cap * sizeof(bool ));
        free(cache);
        free(seen);
    }

    cache    = new_cache;
    seen     = new_seen;
    cache_cap = new_cap;
}

/* Recursive helper that uses memoisation */
static seq_t a_internal(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;

    if (seen[n]) return cache[n];

    idx_t m = n;
    seq_t sum = 0;

    while (m) {
        /* Index of least significant set bit */
        idx_t e = (idx_t)__builtin_ctzll((unsigned long long)m);
        seq_t val = a_internal(e);
        if (val == (seq_t)0) ++sum;
        m &= m - 1;                     /* clear lowest set bit */
    }

    seen[n]  = true;
    cache[n] = sum;
    return sum;
}

/* Public entry point defined by the problem statement */
seq_t a(idx_t n)
{
    grow_cache(n);
    return a_internal(n);
}
