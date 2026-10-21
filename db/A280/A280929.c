/*
 * A280929  Number of steps required to reach the first prime when starting
 *          from n in the Collatz (or '3x+1') problem.
 * https://oeis.org/A280929
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 62, u16 n <= 9262, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = number of Collatz steps required to reach the first prime
   encountered when starting from n.  The first step is always taken,
   so n being prime does not terminate immediately.  Example: a(33)=6. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"   /* bool is_prime_big(seq_t) */

static seq_t *cache = NULL;    /* cache[n] = a(n) or 0 if not yet computed */
static size_t cache_cap = 0;   /* allocated capacity of the cache */

/* Ensure that the cache array can hold indices up to n (inclusive). */
static void ensure_cache(idx_t n)
{
    size_t needed = (size_t)n + 1;          /* indices 0..n */
    if (needed <= cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed)
        new_cap *= 2;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) {                /* allocation failed – no caching */
        free(cache);
        cache = NULL;
        cache_cap = 0;
        return;
    }
    cache = tmp;
    for (size_t i = cache_cap; i < new_cap; ++i)
        cache[i] = 0;
    cache_cap = new_cap;
}

/* Compute a(n).  Returns 0 for n == 0 (unused by OEIS). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Try to use a cached value first.  Cache has been grown if necessary. */
    ensure_cache(n);
    size_t idx = (size_t)n;
    if (cache && cache[idx] != 0)
        return cache[idx];

    seq_t steps = 0;
    seq_t cur = (seq_t)n;

    while (true) {
        /* Perform one Collatz step. */
        if (cur & 1)          /* odd */
            cur = cur * 3 + 1;
        else                  /* even */
            cur >>= 1;

        ++steps;

        /* Check if the new value is prime. */
        if (is_prime_big(cur)) {
            /* Store in cache before returning. */
            if (cache)
                cache[idx] = steps;
            return steps;
        }
    }
}
