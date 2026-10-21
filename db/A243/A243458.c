/*
 * A243458  Primes p such that (p reversed)-3 is also a prime.
 * https://oeis.org/A243458
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth prime p such that reverse(p)−3 is prime
 * Range:     u8 n <= 9, u16 n <= 361, u32 n <= 361, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big, reverse_decimal, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Utility functions provided by the build environment */
#include "../utils/is_prime_big.h"
#include "../utils/reverse_decimal.h"
#include "../utils/seq_realloc.h"

/* Static cache to store the sequence values */
static seq_t *cache = NULL;   /* array of accepted primes     */
static size_t cache_size = 0; /* number of elements stored    */
static size_t cache_cap   = 0; /* allocated capacity           */

/* The largest integer examined so far */
static seq_t last_checked = (seq_t)2; /* start from the first prime */

/* Ensure the cache has at least 'needed' elements */
static void grow_cache(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed)
        new_cap *= 2;
    cache = seq_realloc(cache, cache_cap, new_cap);
    cache_cap = new_cap;
}

/* Fill the cache until it contains at least 'n' terms */
static void extend_cache(idx_t n)
{
    if (cache_size >= (size_t)n)
        return;

    while (cache_size < (size_t)n)
    {
        /* Skip even numbers greater than 2 */
        if (last_checked > (seq_t)2 && (last_checked & (seq_t)1) == (seq_t)0)
        {
            last_checked++;
            continue;
        }

        /* Test primality of the candidate */
        if (is_prime_big(last_checked))
        {
            seq_t rev_val = reverse_decimal(last_checked);
            if (rev_val >= (seq_t)3)
            {
                seq_t rev_minus = rev_val - (seq_t)3;
                if (is_prime_big(rev_minus))
                {
                    grow_cache(cache_size + 1);
                    cache[cache_size++] = last_checked;
                }
            }
        }

        last_checked++;
    }
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;                 /* undefined for indices < 1 */
    extend_cache(n);
    return cache[(size_t)(n - 1)];
}
