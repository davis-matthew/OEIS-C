/*
 * A266966  Primes of the form x^2 + y^2 where x^2 + x*y + y^2 is also prime
 *          with positive x and y.
 * https://oeis.org/A266966
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   floor_sqrt, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A266966: primes p such that p = x^2 + y^2 (x>0,y>0)
   and q = x^2 + x*y + y^2 is also prime.
   The function finds terms in increasing order, caching
   results for repeated calls. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"
#include "../utils/floor_sqrt.h"

/* cached sequence – one‑based externally */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t tested = 1;   /* last integer examined */

/* Ensure that the cache has capacity for at least `need` entries */
static void ensure_cache_cap (size_t need)
{
    if (cache_cap < need) {
        size_t newcap = cache_cap ? cache_cap * 2 : 16;
        while (newcap < need) newcap <<= 1;
        cache = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        cache_cap = newcap;
    }
}

/* Append a new value to the cache */
static void cache_append (seq_t val)
{
    ensure_cache_cap(cache_len + 1);
    cache[cache_len++] = val;
}

seq_t a(idx_t n)
{
    /* Lazy initialisation of the cache */
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache_len = 0;
    }

    /* Generate terms until we have at least n of them */
    while (cache_len < n) {
        tested++;

        /* Only test primes; the helper works for any seq_t size */
        if (!is_prime_big((seq_t)tested))
            continue;

        seq_t p = (seq_t)tested;           /* candidate prime            */
        seq_t lim = floor_sqrt(p);         /* largest possible x value   */
        bool good = false;

        /* Search for a representation p = x^2 + y^2 with x,y>0.
           If such x,y exist, check whether q = x^2 + xy + y^2 is prime. */
        for (idx_t ix = 1; ix <= (idx_t)lim; ++ix) {
            seq_t x = (seq_t)ix;
            seq_t x2 = x * x;
            seq_t y2 = p - x2;
            if (y2 == 0) continue;        /* y would be zero */
            seq_t y = floor_sqrt(y2);
            if (y == 0) continue;        /* y would be zero */
            if (y * y != y2) continue;   /* y^2 not exact */

            seq_t q = x2 + x * y + y * y;
            if (is_prime_big(q)) {
                good = true;
                break;
            }
        }

        if (good)
            cache_append(p);
    }

    /* a(1) corresponds to cache[0] */
    return cache[n - 1];
}
