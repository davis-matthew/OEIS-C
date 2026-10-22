/*
 * A323067  Primes whose binary complement (A035327) is a square.
 * https://oeis.org/A323067
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   floor_sqrt, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A323067: primes whose binary complement is a perfect square. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* static cache of found primes (uint64_t) */
    static uint64_t cache[20000];
    static idx_t cache_len = 0;
    static uint64_t last_checked = 2;

    if (n == (idx_t)0) return 0;   /* domain starts at 1, but guard */

    while (cache_len < n) {
        uint64_t p = last_checked;
        /* find next prime */
        while (!is_prime(p)) ++p;

        /* compute bit length of p */
        unsigned bitlen = 0;
        uint64_t tmp = p;
        while (tmp) { tmp >>= 1; ++bitlen; }

        /* construct mask with bitlen 1s without shifting 64 bits */
        seq_t all_ones = (((seq_t)1 << (bitlen-1)) * (seq_t)2) - (seq_t)1;

        /* binary complement of p */
        seq_t complement = all_ones ^ (seq_t)p;

        /* test for perfect square */
        seq_t root = floor_sqrt(complement);
        if (root * root == complement) {
            /* store prime in cache */
            cache[cache_len++] = p;
        }

        /* advance last_checked for next search */
        last_checked = p + 1;
    }

    return (seq_t)cache[n-1];
}
