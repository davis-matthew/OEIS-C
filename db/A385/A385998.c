/*
 * A385998  Numbers that are divisible by an equal number of distinct primes
 *          and squares.
 * https://oeis.org/A385998
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 101, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   A(n) is the n‑th positive integer whose number of distinct prime
   divisors equals its number of distinct square divisors.
   The sequence consists of all prime numbers and additional composite
   numbers that satisfy: 
        #primes(n) = Π_i (⌊e_i/2⌋ + 1)
   where n = ∏ p_i^{e_i}.  
   Here we compute the sequence by iterating over successive integers,
   factorising each with factor_u64, and collecting those that meet the
   condition.  Results are cached so that calls are independent of
   order.
*/
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    static uint64_t *cache = NULL;
    static size_t cache_len = 0;   /* number of terms stored */
    static size_t cache_cap = 0;   /* allocated capacity */
    static uint64_t next_candidate = 2;  /* integer to test next */

    /* allocate small cache if needed */
    if (cache == NULL) {
        cache_cap = 64;
        cache = (uint64_t *)malloc(cache_cap * sizeof(uint64_t));
    }
    /* generate terms until we have at least n of them */
    while (cache_len < (size_t)n) {
        uint64_t m = next_candidate++;

        /* factorise m */
        uint64_t pr[64], ex[64];
        size_t cnt;
        factor_u64(m, pr, ex, &cnt);

        /* compute number of distinct square divisors:
           product over primes of (floor(e/2)+1)              */
        uint64_t squares = 1;
        for (size_t i = 0; i < cnt; ++i) {
            squares *= (ex[i] / 2 + 1);
        }

        /* accept if distinct prime count == squares count */
        if (cnt == squares) {
            /* grow cache if needed */
            if (cache_len == cache_cap) {
                cache_cap *= 2;
                cache = (uint64_t *)realloc(cache, cache_cap * sizeof(uint64_t));
            }
            cache[cache_len++] = m;
        }
    }

    /* return the requested term, cast to seq_t */
    return (seq_t)cache[n - 1];
}
