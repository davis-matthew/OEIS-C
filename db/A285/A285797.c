/*
 * A285797  Number of ways to write n as an ordered sum of two numbers that
 *          are the product of an odd number of distinct primes.
 * https://oeis.org/A285797
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{x | 1 ≤ x ≤ n-1, x and n-x are squarefree with odd Ω}
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Utilities for prime generation */
#include "../utils/build_primes.h"

static idx_t      cache_max_n   = 0;
static uint8_t   *good_cache    = NULL;   /* 0 = not good, 1 = good */
static size_t     good_cache_cap = 0;

/* Extend the cache up to n (inclusive). */
static void extend_cache_to(idx_t n)
{
    if (n <= cache_max_n)
        return; /* already cached */

    /* Allocate or grow the cache array. */
    if ((size_t)(n + 1) > good_cache_cap) {
        good_cache = (uint8_t *)realloc(good_cache, (size_t)(n + 1) * sizeof *good_cache);
        good_cache_cap = n + 1;
    }

    /* Build list of primes up to n. */
    idx_t *primes = NULL;
    size_t primes_cnt = 0;
    build_primes(n, &primes, &primes_cnt);

    /* Compute good[n] for all new indices. */
    for (idx_t i = cache_max_n + 1; i <= n; ++i) {
        if (i < 2) {
            good_cache[i] = 0;
            continue;
        }
        idx_t x = i;
        bool squarefree = true;
        bool oddcount   = false; /* true if Ω(x) is odd */

        for (size_t pi = 0; pi < primes_cnt && primes[pi] * primes[pi] <= x; ++pi) {
            idx_t p = primes[pi];
            if (x % p == 0) {
                uint8_t cnt = 0;
                do {
                    x /= p;
                    ++cnt;
                } while (x % p == 0);
                if (cnt > 1) { /* not squarefree */
                    squarefree = false;
                    break;
                }
                oddcount = !oddcount;
            }
        }
        if (squarefree && x > 1) { /* remaining prime factor ≤ n */
            oddcount = !oddcount;
        }

        good_cache[i] = (squarefree && oddcount) ? 1 : 0;
    }

    /* Ensure base cases for 0 and 1. */
    good_cache[0] = 0;
    good_cache[1] = 0;

    cache_max_n = n;
    free(primes);
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* a(0) = a(1) = a(2) = a(3) = 0 by definition.   */
    if (n <= 1)
        return (seq_t)0;

    extend_cache_to(n);

    seq_t count = 0;
    for (idx_t x = 1; x < n; ++x) {
        if (good_cache[x] && good_cache[n - x])
            ++count;
    }
    return count;
}
