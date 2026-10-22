/*
 * A303554  Union of the prime powers (p^k, p prime, k >= 0) and numbers that
 *          are the product of 2 or more distinct primes.
 * https://oeis.org/A303554
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth integer that is either a prime power or a squarefree
 *            composite (product of at least two distinct primes). 1 is
 *            considered part of the sequence
 * Range:     u8 n <= 172, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Helper: test whether x (uint64_t) satisfies the property */
static bool is_valid(uint64_t x)
{
    if (x == 1) return true;          /* 1 is included by definition */
    uint64_t temp = x;
    size_t distinct = 0;              /* number of distinct prime factors */
    size_t repeated = 0;              /* number of factors with exponent > 1 */

    /* trial division up to sqrt(temp) */
    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            ++distinct;
            size_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            if (e > 1) ++repeated;
            if (temp == 1) break;
        }
    }
    if (temp > 1) {                   /* remaining prime factor */
        ++distinct;
        /* exponent 1, so repeated unchanged */
    }

    /* Condition: allowed if squarefree (repeated == 0)
       or if it's a prime power (repeated == 1 and only one distinct factor). */
    if (repeated == 0) return true;
    if (repeated == 1 && distinct == 1) return true;
    return false;
}

static seq_t *cache = NULL;                /* dynamic cache of sequence values */
static size_t cache_len = 0;               /* number of values stored */
static size_t cache_cap = 0;               /* allocated capacity */
static uint64_t next_candidate = 1;        /* next integer to test */

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined below offset, but protect */
    /* Grow cache until we have at least n terms */
    while (cache_len < (size_t)n) {
        /* Find next valid number */
        while (!is_valid(next_candidate)) ++next_candidate;

        /* Store it */
        if (cache_len >= cache_cap) {
            size_t newcap = cache_cap == 0 ? 4 : cache_cap << 1;
            seq_t *newptr = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
            if (!newptr) { /* allocation failure – return 0 (unlikely) */
                free(cache);
                cache = NULL;
                cache_len = cache_cap = 0;
                return 0;
            }
            cache = newptr;
            cache_cap = newcap;
        }
        cache[cache_len++] = (seq_t)next_candidate;
        ++next_candidate;
    }
    return cache[n - 1]; /* offset 1 => a(1) is first element */
}
