/*
 * A296925  Inert rational primes in the field Q(sqrt(-10)).
 * https://oeis.org/A296925
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 3295, u32 n <= 1911, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A296925: primes congruent to 3,17,21,27,29,31,33,39 mod 40
   (primes for which (-10|p) = -1).
   Computes the sequence by testing successive odd numbers for primality
   and selecting those with the desired residues. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* static cache of found primes */
    static seq_t *cache      = NULL;
    static size_t cache_len  = 0;
    static size_t cache_cap  = 0;
    /* next odd integer to test (start from 3) */
    static uint64_t next_candidate = 3;

    if (n == 0)  /* OEIS offset starts at 1; return 0 for safety */
        return (seq_t)0;

    /* gather primes until we have at least n of them */
    while (cache_len < n) {
        /* test primality */
        if (is_prime_big((seq_t)next_candidate)) {
            uint64_t mod40 = next_candidate % 40ULL;
            if (mod40 == 3 || mod40 == 17 || mod40 == 21 ||
                mod40 == 27 || mod40 == 29 || mod40 == 31 ||
                mod40 == 33 || mod40 == 39) {

                /* grow cache if necessary */
                if (cache_len == cache_cap) {
                    size_t new_cap = cache_cap ? cache_cap << 1 : 8;
                    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    cache      = tmp;
                    cache_cap  = new_cap;
                }
                cache[cache_len++] = (seq_t)next_candidate;
            }
        }
        next_candidate += 2;   /* only odd numbers need be tested */
    }
    return cache[n - 1];
}
