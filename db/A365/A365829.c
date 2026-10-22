/*
 * A365829  Squarefree non-semiprimes.
 * https://oeis.org/A365829
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A365829: squarefree integers that are not the product of two
   distinct primes (i.e. 1, primes, or square‑free composites with at
   least three distinct prime factors).
   The function iterates over natural numbers, keeping a cache of the
   first few sequence values.  Each candidate is tested for squarefreeness
   and for having exactly two distinct prime factors.  When a new value
   satisfying the criteria is found it is appended to the cache.  The
   search continues until enough values have been accumulated to answer
   the requested index. */
#include <stdlib.h>
#include <stdbool.h>

/* Return true iff x is squarefree and not a product of exactly two
   distinct primes. */
static bool is_allowed(idx_t x)
{
    if (x == 1) return true;            /* 1 is allowed */

    idx_t temp = x;
    idx_t primes = 0;                   /* number of distinct prime factors */

    /* handle factor 2 */
    if ((temp & 1) == 0) {               /* temp divisible by 2 */
        primes++;
        temp >>= 1;                     /* divide by 2 */
        if ((temp & 1) == 0) return false; /* square factor 2^2 detected */
    }

    /* handle odd factors */
    for (idx_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            primes++;
            temp /= p;
            if (temp % p == 0) return false; /* square factor detected */
        }
    }

    if (temp > 1) {                      /* remaining prime factor */
        primes++;
    }

    return primes != 2;                  /* discard squarefree semiprimes */
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;          /* cache of found sequence values */
    static size_t cache_len = 0;         /* number of cached values */
    static size_t cache_cap = 0;         /* capacity of cache array */
    static idx_t next_candidate = 1;     /* next integer to examine */

    /* Extend cache until it contains at least n values */
    while (cache_len < (size_t)n) {
        if (is_allowed(next_candidate)) {
            /* insert value into cache */
            seq_t val = (seq_t)next_candidate;
            if (cache_len == cache_cap) {
                cache_cap = (cache_cap == 0) ? 4 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (!tmp) return 0;      /* allocation failure (unlikely) */
                cache = tmp;
            }
            cache[cache_len++] = val;
        }
        ++next_candidate;
    }

    return cache[(size_t)(n - 1)];
}
