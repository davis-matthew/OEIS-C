/*
 * A381950  Odd numbers whose prime factorization has an even maximum
 *          exponent.
 * https://oeis.org/A381950
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 4953, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* a(n) = n-th odd integer whose largest exponent in the prime factorization
   is even. */
seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    /* next odd number to examine */
    static uint64_t next_candidate = 1;

    /* guard against the (unlikely) case that n=0 is passed */
    if (n == 0) return 0;

    /* ensure enough terms in the cache */
    while (cache_len < n) {
        /* test current odd candidate */
        uint64_t x = next_candidate;
        int max_exp = 0;
        uint64_t temp = x;

        /* factorization by trial division */
        for (uint64_t p = 2; p * p <= temp; ++p) {
            if (temp % p == 0) {
                int exp = 0;
                while (temp % p == 0) {
                    temp /= p;
                    ++exp;
                }
                if (exp > max_exp)
                    max_exp = exp;
            }
        }
        if (temp > 1 && 1 > max_exp)          /* remaining prime factor */
            max_exp = 1;

        /* add to cache if the condition holds */
        if ((max_exp & 1) == 0) {             /* even maximum exponent */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 128;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) exit(1);            /* allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)next_candidate;
        }

        /* go to the next odd number */
        next_candidate += 2;
    }

    return cache[n - 1];
}
