/*
 * A393914  Numbers k such that A008472(k) is coprime to A001414(k).
 * https://oeis.org/A393914
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 58, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by trial division factorisation.
   a(n) = the n-th integer k > 1 for which
   gcd( sum of distinct prime factors of k,
        sum of prime factors of k with multiplicity )
   equals 1.
   The implementation keeps a dynamic cache; factorisation uses
   simple trial division.  Works for n up to at least 10000 in < 2 s.  */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/gcd.h"              /* gcd(seq_t, seq_t) */

static seq_t *cache   = NULL;   /* cached sequence values          */
static size_t cache_len = 0;    /* number of values stored        */
static size_t cache_cap = 0;    /* allocated capacity              */
static uint64_t next_k  = 2;    /* next integer to examine        */

/* Grow the cache array if necessary. */
static void ensure_cache(idx_t need)
{
    if (cache_cap < need) {
        size_t new_cap = cache_cap == 0 ? 256 : cache_cap << 1;
        if (new_cap < need) new_cap = need;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Generate further terms until we have 'need' elements. */
    while (cache_len < need) {
        uint64_t k = next_k++;
        uint64_t sum_mul = 0;      /* sum with multiplicity          */
        uint64_t sum_distinct = 0; /* sum of distinct prime factors */
        uint64_t last = 0;         /* last distinct prime seen       */
        uint64_t n = k;

        /* factor 2 separately */
        while ((n & 1) == 0) {
            sum_mul += 2;
            if (last != 2) { sum_distinct += 2; last = 2; }
            n >>= 1;
        }

        /* factor odd primes */
        for (uint64_t p = 3; p * p <= n; p += 2) {
            if (n % p == 0) {
                while (n % p == 0) {
                    sum_mul += p;
                    if (last != p) { sum_distinct += p; last = p; }
                    n /= p;
                }
            }
        }

        /* n is now 1 or a prime > sqrt(original n) */
        if (n > 1) {
            sum_mul += n;
            if (last != n) { sum_distinct += n; last = n; }
        }

        if (gcd((seq_t)sum_distinct, (seq_t)sum_mul) == (seq_t)1) {
            cache[cache_len++] = (seq_t)k;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;       /* offset is 1, undefined for n<1 */
    ensure_cache(n);
    return cache[n - 1];
}
