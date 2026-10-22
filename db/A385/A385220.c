/*
 * A385220  Primes p such that multiplicative order of 3 modulo p is odd.
 * https://oeis.org/A385220
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime p such that ord_p(3) is odd Sequence A385220
 * Range:     u8 n <= 19, u16 n <= 2206, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_small, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_small.h"
#include "../utils/mod_pow.h"

/* Compute a(n): nth prime p whose multiplicative order of 3 modulo p is odd */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined for offset 0 */

    /* Static cache of sequence values */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;

    /* Next candidate odd number to test (after 2) */
    static uint64_t next_candidate = 3;

    /* Ensure cache has at least n terms */
    while (cache_len < (size_t)n) {
        /* Handle the first term 2 */
        if (cache_len == 0) {
            if (cache_cap == 0) {
                cache_cap = 16;
                cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = (seq_t)2;
            continue;
        }

        /* Find next prime p satisfying the property */
        uint64_t cand = next_candidate;
        for (;;) {
            /* Skip even numbers */
            if ((cand & 1) == 0) {
                cand++;
                continue;
            }

            /* 3 is not accepted because 3 does not have a multiplicative order of 3 modulo 3 */
            if (cand == 3) {
                cand += 2;
                continue;
            }

            /* Primality test */
            if (!is_prime_small((seq_t)cand)) {
                cand += 2;
                continue;
            }

            /* Compute odd part of p-1: m = (p-1) >> v2(p-1) */
            seq_t p_seq = (seq_t)cand;
            seq_t odd_part = p_seq - 1;
            while ((odd_part & 1) == 0)
                odd_part >>= 1;

            /* Check if 3^odd_part ≡ 1 (mod p) */
            uint64_t exp = (uint64_t)odd_part;  /* safe for the range we need */
            if (mod_pow(3, exp, p_seq) == 1) {
                /* p qualifies */
                if (cache_len == cache_cap) {
                    cache_cap = cache_cap ? cache_cap * 2 : 16;
                    cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
                }
                cache[cache_len++] = p_seq;
                next_candidate = cand + 2;  /* start after this prime */
                break;
            }

            cand += 2;                    /* try next odd candidate */
        }
    }

    return cache[(size_t)n - 1];
}
