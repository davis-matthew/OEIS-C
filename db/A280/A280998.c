/*
 * A280998  Numbers with a prime number of 1's in their binary reflected Gray
 *          code representation.
 * https://oeis.org/A280998
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 148, u16 n <= 9743, u32 n <= 9743, u64 n <= 9743,
 *            u128 n <= 9743, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers x such that popcount(x XOR (x >> 1)) is prime. */
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Lazy initialization of prime lookup up to 128 bits */
    static bool prime_table[129] = {0};
    static bool prime_table_initialized = false;
    if (!prime_table_initialized) {
        static const int primes[] = {
            2,3,5,7,11,13,17,19,23,29,31,
            37,41,43,47,53,59,61,67,71,73,
            79,83,89,97,101,103,107,109,113,127
        };
        const size_t pcount = sizeof(primes)/sizeof(primes[0]);
        for (size_t i = 0; i < pcount; ++i)
            prime_table[primes[i]] = true;
        prime_table_initialized = true;
    }

    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_capacity = 0;
    static seq_t next_candidate = 1;

    /* Return cached value if already computed */
    if (n <= cache_size)
        return cache[n-1];

    /* Allocate initial cache */
    if (!cache) {
        cache_capacity = 64;
        cache = (seq_t*)malloc(sizeof(seq_t) * cache_capacity);
        if (!cache) abort();   /* critical failure */
    }

    /* Grow cache and compute values until we reach n */
    while (cache_size < n) {
        if (cache_size == cache_capacity) {
            size_t new_cap = cache_capacity * 2;
            seq_t *tmp = (seq_t*)realloc(cache, sizeof(seq_t) * new_cap);
            if (!tmp) abort();  /* critical failure */
            cache = tmp;
            cache_capacity = new_cap;
        }

        seq_t candidate = next_candidate;
        /* Find next number whose Gray code has a prime number of ones */
        while (1) {
            seq_t g = candidate ^ (candidate >> 1);
            unsigned count = 0;
            seq_t t = g;
            while (t) {
                t &= t - 1;
                ++count;
            }
            if (count <= 128 && prime_table[count]) {
                cache[cache_size++] = candidate;
                next_candidate = candidate + 1;
                break;
            }
            ++candidate;
        }
    }

    return cache[n-1];
}
