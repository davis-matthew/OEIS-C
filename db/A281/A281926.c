/*
 * A281926  Numbers that are the product of exactly 5 primes and are of the
 *          form prime(k) + prime(k + 1).
 * https://oeis.org/A281926
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 862, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_prime_factors, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A281926: numbers that are the sum of two consecutive primes and
   have exactly five prime factors (counted with multiplicity). */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/count_prime_factors.h"

static seq_t *cache   = NULL; /* stored terms */
static size_t cache_cap = 0;   /* allocated size of cache   */
static size_t cache_len = 0;   /* number of terms stored   */
static idx_t  next_k   = 1;    /* next prime index to examine */

seq_t a(idx_t n)
{
    if (n == 0)                     /* offset is 1, undefined for n=0 */
        return 0;

    /* Grow cache until it contains at least n terms */
    while (cache_len < (size_t)n) {
        seq_t pk  = (seq_t)get_prime(next_k);
        seq_t pk1 = (seq_t)get_prime(next_k + 1);
        seq_t sum = pk + pk1;

        if (count_prime_factors(sum) == 5) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 64;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp)
                    return 0;           /* allocation failed */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = sum;
        }
        ++next_k;                    /* advance to next consecutive primes pair */
    }
    return cache[n - 1];
}
