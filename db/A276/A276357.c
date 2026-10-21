/*
 * A276357  Primes of the form (p*2^x-1)/3, where p is also prime and x is a
 *          positive integer.
 * https://oeis.org/A276357
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of sequence A276357: primes a such that */
/* 3*a + 1 = p * 2^x with prime p (p may be 2) and x >= 1. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* Static cache for previously found terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of terms stored */
    static size_t cache_cap = 0;
    static seq_t next_a = 3;       /* next odd candidate to test */

    /* Grow the cache on first use */
    if (cache == NULL) {
        cache_cap = 128;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* Compute terms up to the requested index if necessary */
    while (cache_len < (size_t)n) {
        seq_t a_val = next_a;
        seq_t m = ((seq_t)3 * a_val) + 1;   /* 3*a + 1 */

        if ((m & ((seq_t)1)) == 0) {        /* require m even (x >= 1) */
            seq_t r = m;
            /* strip all factors of 2: r becomes the odd part of m */
            while ((r & ((seq_t)1)) == 0) {
                r >>= 1;
            }
            /* a must be prime */
            if (is_prime_big(a_val)) {
                /* accept if the odd part is prime or equals 1 (p = 2 case) */
                if (r == 1 || is_prime_big(r)) {
                    /* store the term in the cache */
                    if (cache_len == cache_cap) {     /* grow if necessary */
                        cache_cap *= 2;
                        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                    }
                    cache[cache_len++] = a_val;
                }
            }
        }

        next_a += 2;  /* advance to the next odd number */
    }

    /* Return the cached term */
    return cache[n - 1];
}
