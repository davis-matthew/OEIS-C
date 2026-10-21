/*
 * A272636  a(0)=0, a(1)=1; thereafter a(n) = squarefree part of
 *          a(n-1)+a(n-2).
 * https://oeis.org/A272636
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = squarefree_part(a(n-1)+a(n-2)), a(0)=0, a(1)=1 where
 *            squarefree_part(x) = x divided by its largest square divisor
 *            (i.e. the product of distinct primes of x raised to odd powers)
 * Range:     u8 n <= 115, u16 n <= 115, u32 n <= 115, u64 n <= 115,
 *            u128 n <= 115, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;    /* cache of precomputed values */
static size_t cache_len = 0;   /* number of elements actually stored */

/* Return the squarefree part of x: x divided by the largest square divisor. */
static seq_t squarefree_part(seq_t x) {
    if (x <= 1)
        return x;
    /* We know all values that appear in this sequence are small, so
       casting to uint64_t is safe. */
    uint64_t v = (uint64_t)x;
    seq_t res = 1;
    for (uint64_t p = 2; p * p <= v; ++p) {
        if (v % p == 0) {
            int exp = 0;
            while (v % p == 0) {
                v /= p;
                exp++;
            }
            if (exp & 1)      /* odd exponent */
                res *= (seq_t)p;
        }
    }
    if (v > 1)                /* remaining prime factor has exponent 1 (odd) */
        res *= (seq_t)v;
    return res;
}

/* Compute a(n) for any n >= 0.  Results are memoised in a static cache. */
seq_t a(idx_t n) {
    /* Ensure cache has entries for 0 and 1 */
    if (cache_len == 0) {
        cache_len = 2;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_len);
        cache[0] = 0;
        cache[1] = 1;
    }

    /* Grow cache as needed */
    if ((size_t)n >= cache_len) {
        size_t new_len = (size_t)n + 1;
        cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        /* Compute missing values */
        for (size_t i = cache_len; i < new_len; ++i) {
            seq_t sum = cache[i - 1] + cache[i - 2];
            cache[i] = squarefree_part(sum);
        }
        cache_len = new_len;
    }

    return cache[(size_t)n];
}
