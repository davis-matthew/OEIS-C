/*
 * A285693  a(1) = 2; a(n + 1) = smallest prime > a(n) such that a(n + 1) -
 *          a(n) is the product of 8 primes.
 * https://oeis.org/A285693
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 33, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   count_prime_factors, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A285693
   a(1) = 2
   a(n+1) = smallest prime > a(n) such that a(n+1)-a(n) has exactly 8 prime factors (counting multiplicities). */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/count_prime_factors.h"

static inline bool product_of_8_primes(seq_t diff)
{
    return count_prime_factors(diff) == (seq_t)8;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t computed = 0;   /* number of terms stored in cache */

    if (n > computed)
    {
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * (size_t)n);
        if (!tmp)
            return (seq_t)0;    /* allocation failure */
        cache = tmp;

        if (computed == 0)
        {
            cache[0] = (seq_t)2;
            computed = 1;
        }

        for (idx_t i = computed + 1; i <= n; ++i)
        {
            seq_t prev = cache[computed - 1];
            seq_t cand = prev + (seq_t)1;
            for (;;)
            {
                while (!is_prime_big(cand))
                    cand = cand + (seq_t)1;
                seq_t diff = cand - prev;
                if (product_of_8_primes(diff))
                    break;
                cand = cand + (seq_t)1;
            }
            cache[computed] = cand;
            ++computed;
        }
    }

    return cache[n - 1];
}
