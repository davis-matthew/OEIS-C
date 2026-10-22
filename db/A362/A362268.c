/*
 * A362268  Numbers whose prime factors counted with multiplicity satisfy:
 *          (maximum) - (minimum) = (mean).
 * https://oeis.org/A362268
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 32, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers whose prime factors counted with multiplicity satisfy:
   (maximum) - (minimum) = (mean).  Indexed from 1. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <inttypes.h>

/* Helper: test whether an integer n satisfies the sequence condition
   (max - min) == mean of its prime factors counted with multiplicity. */
static bool satisfies(idx_t n)
{
    if (n < 2)
        return false;

    idx_t x = n;
    seq_t sum = 0;
    seq_t k   = 0;          /* total number of prime factors (with multiplicity) */
    idx_t minp = 0;
    idx_t maxp = 0;

    /* factor 2 */
    if ((x & 1) == 0) {
        uint32_t e = 0;
        while ((x & 1) == 0) {
            x >>= 1;
            ++e;
        }
        sum += ((seq_t)2) * e;
        k   += e;
        if (minp == 0) {
            minp = 2;
        }
        maxp = 2;
    }

    /* factor odd primes */
    for (idx_t p = 3; p * p <= x; p += 2) {
        if (x % p == 0) {
            uint32_t e = 0;
            while (x % p == 0) {
                x /= p;
                ++e;
            }
            sum += ((seq_t)p) * e;
            k   += e;
            if (minp == 0) {
                minp = p;
            }
            maxp = p;
        }
    }

    /* remaining prime factor */
    if (x > 1) {
        idx_t p = x;
        sum += ((seq_t)p);
        k   += 1;
        if (minp == 0) {
            minp = p;
        }
        maxp = p;
    }

    if (k == 0)
        return false;

    /* condition: mean must be integer and equal to maxp - minp */
    if (sum % k != 0)
        return false;
    seq_t mean = sum / k;
    return ((seq_t)(maxp - minp) == mean);
}

/* Main sequence function.  It caches the sequence in a static array,
   computing new terms on demand. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* static cache */
    static seq_t *cache       = NULL;
    static size_t cache_len   = 0;
    static size_t cache_cap   = 0;
    static idx_t next_candidate = 2;   /* next integer to test */

    /* grow cache as needed */
    while (cache_len < (size_t)n) {
        idx_t cand = next_candidate++;
        if (satisfies(cand)) {
            if (cache_len == cache_cap) {
                cache_cap = (cache_cap == 0) ? 8 : (cache_cap * 2);
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = (seq_t)cand;
        }
    }

    return cache[n - 1];
}
