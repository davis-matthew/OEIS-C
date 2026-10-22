/*
 * A386325  Primes without {0, 7} as digits.
 * https://oeis.org/A386325
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the nth prime whose decimal digits are all ≠0,≠7 (OEIS
 *            A386325)
 * Range:     u8 n <= 33, u16 n <= 2578, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isPrimeTrial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard headers */
#include <stdbool.h>
#include <stdlib.h>

/* Assume utils functions are available; we use isPrimeTrial for simplicity */
#include "../utils/isPrimeTrial.h"

/* Static cache for memoization */
static seq_t *cache = NULL;          /* array of found primes */
static size_t cache_cap = 0;         /* current capacity of cache */
static idx_t cache_cnt = 0;          /* number of primes stored */
static seq_t last_candidate = 1;     /* last tested candidate */

/* Return true if x has no decimal digit 0 or 7 */
static bool digits_ok(seq_t x)
{
    while (x > 0) {
        unsigned int d = (unsigned int)(x % 10u);
        if (d == 0u || d == 7u)
            return false;
        x /= 10u;
    }
    return true;
}

/* Sequence generator function */
seq_t a(idx_t n)
{
    /* Ensure we have at least n terms in the cache */
    while (cache_cnt < n) {
        /* Next candidate integer */
        last_candidate++;

        /* Skip numbers whose digits contain 0 or 7 */
        while (!digits_ok(last_candidate))
            last_candidate++;

        /* primality test */
        if (isPrimeTrial((seq_t)last_candidate)) {
            /* grow cache if needed */
            if (cache_cnt >= cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 64 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) { /* allocation failed – abort */
                    abort();
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_cnt++] = (seq_t)last_candidate;
        }
    }
    return cache[n - 1];
}
