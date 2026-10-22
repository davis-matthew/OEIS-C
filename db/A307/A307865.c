/*
 * A307865  a(n) is the number of natural bases b < 2n+1 such that b^n == -1
 *          (mod 2n+1).
 * https://oeis.org/A307865
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 259, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A307865:
   a(n) = number of integers b with 0 ≤ b < 2n+1 such that
   b^n ≡ -1 (mod 2n+1).  The function caches computed
   values for repeated calls. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mod_pow.h"    /* provides seq_t mod_pow(seq_t, uint64_t, seq_t); */

static seq_t *cache = NULL;      /* cache[n] holds a(n) or (seq_t)-1 if not yet computed */
static idx_t cache_cap = 0;      /* current capacity of the cache array */

static void ensure_cache(idx_t n)
{
    if (n >= cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failed – abort the program (should never happen). */
            abort();
        }
        cache = tmp;
        for (idx_t i = cache_cap; i < new_cap; ++i)
            cache[i] = (seq_t)-1;   /* sentinel for "not computed" */
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    if (cache[n] != (seq_t)-1)
        return cache[n];

    /* Special case n == 0 */
    if (n == 0) {
        cache[0] = 0;
        return 0;
    }

    seq_t m = (seq_t)(2 * (seq_t)n + 1);   /* modulus 2n+1 */
    seq_t target = m - 1;                  /* -1 modulo m */
    seq_t cnt = 0;

    for (seq_t b = 0; b < m; ++b) {
        /* mod_pow returns b^n mod m */
        seq_t r = mod_pow(b, (uint64_t)n, m);
        if (r == target)
            ++cnt;
    }

    cache[n] = cnt;
    return cnt;
}
