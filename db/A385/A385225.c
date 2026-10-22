/*
 * A385225  Primes p such that multiplicative order of -5 modulo p is odd.
 * https://oeis.org/A385225
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 2169, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 6461, big any n
 * Helpers:   build_primes_upto, grow_seq, mod_pow, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes p such that multiplicative order of -5 modulo p is odd */
/* Computation: iterate primes via next_prime, test the order using
   mod_pow and factorisation, memoise found terms up to n. */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/mod_pow.h"
#include "../utils/next_prime.h"
#include "../utils/build_primes_upto.h"
#include "../utils/grow_seq.h"

/* Check whether the multiplicative order of -5 mod p is odd. */
static bool odd_order_minus5(uint64_t p, uint64_t *prime_list, size_t prime_cnt)
{
    /* Skip p = 5 (non‑invertible) and handle trivial primes */
    if (p == 2 || p == 3) return true;
    if (p == 5) return false;

    /* g = (-5) mod p  (non‑zero for p > 5) */
    uint64_t g = p - 5;   /* p > 5 */

    /* Compute ord = p-1, then minimize it */
    uint64_t ord = p - 1;
    uint64_t tmp = ord;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t q = prime_list[i];
        if (q * q > tmp) break;
        if (tmp % q == 0) {
            while (tmp % q == 0) tmp /= q;
            while (ord % q == 0) {
                uint64_t cand = ord / q;
                if (mod_pow((seq_t)g, cand, (seq_t)p) == (seq_t)1)
                    ord = cand;
                else
                    break;
            }
        }
    }
    if (tmp > 1) { /* remaining prime factor */
        uint64_t q = tmp;
        while (ord % q == 0) {
            uint64_t cand = ord / q;
            if (mod_pow((seq_t)g, cand, (seq_t)p) == (seq_t)1)
                ord = cand;
            else
                break;
        }
    }
    return (ord & 1ULL) == 1;
}

seq_t a(idx_t n)
{
    /* Static memoisation state */
    static seq_t *cache = NULL;
    static size_t cache_len = 0, cache_cap = 0;
    static uint64_t last_prime = 1;

    /* Prime list for factorisation, built once */
    static uint64_t *prime_list = NULL;
    static size_t prime_cnt = 0;
    if (!prime_list) {
        /* Upper limit large enough for n up to ≈100 000 */
        const size_t limit = 2000000;
        build_primes_upto(limit, &prime_list, &prime_cnt);
    }

    /* Produce terms until we have the n‑th one */
    while (cache_len < (size_t)n) {
        last_prime = next_prime(last_prime);
        if (odd_order_minus5(last_prime, prime_list, prime_cnt)) {
            if (cache_len >= cache_cap)
                grow_seq(&cache, &cache_cap, 1);
            cache[cache_len++] = (seq_t)last_prime;
        }
    }

    return cache[(size_t)n - 1];
}
