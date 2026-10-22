/*
 * A400684  Primes p such that p+2 and (p-1)^2+1 are also prime.
 * https://oeis.org/A400684
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, new
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 5, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Computes OEIS A400684: the primes p such that p+2 and (p-1)^2+1 are also prime.
 * The algorithm incrementally tests successive odd integers for primality (using the
 * provided helper is_prime_big) and stores the qualifying primes in a dynamically
 * sized cache.  It can answer requests for any index n ≥ 1 (ignoring n = 0) in
 * any order; subsequent calls reuse the cached results.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"   /* boolean is_prime_big(seq_t n) */

static seq_t *cache   = NULL;   /* cache of found terms */
static size_t cache_len   = 0;
static size_t cache_cap   = 0;

static uint64_t next_candidate = 3;   /* next odd integer to test for primality */

/* Grow the cache array to hold at least need elements */
static void grow_cache(size_t need)
{
    if (cache_cap >= need) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap < need) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) return;              /* out‑of‑memory: abandon growth */
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined for n < 1, safe guard */

    /* Generate terms until we have at least n entries */
    while (cache_len < (size_t)n) {
        /* Test candidate for primality */
        if (is_prime_big((seq_t)next_candidate)) {
            seq_t p = (seq_t)next_candidate;

            /* Check twin prime: p + 2 */
            seq_t p2 = p + (seq_t)2;
            if (!is_prime_big(p2)) {
                next_candidate += 2;
                continue;
            }

            /* Check quadratic condition: (p-1)^2 + 1 */
            seq_t d   = p - (seq_t)1;
            seq_t val = d * d + (seq_t)1;
            if (!is_prime_big(val)) {
                next_candidate += 2;
                continue;
            }

            /* Store qualifying prime */
            grow_cache(cache_len + 1);
            cache[cache_len++] = p;
        }
        next_candidate += 2;   /* move to next odd number */
    }

    return cache[(size_t)n - 1];
}
