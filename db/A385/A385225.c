/*
 * A385225  Primes p such that multiplicative order of -5 modulo p is odd.
 * https://oeis.org/A385225
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 2169, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 75 terms through n = 9137 (2026-11-03)
 * Helpers:   build_primes_upto, mem, next_prime, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes p such that multiplicative order of -5 modulo p is odd */
/* Computation: iterate primes via next_prime, test the order using
   mod_pow and factorisation, memoise found terms up to n. */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/build_primes_upto.h"
#include "../utils/next_prime.h"
#include "../utils/powmod.h"
#include "../utils/mem.h"

/* --- dynamic array helpers ------------------------------------------- */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *arr, (new_cap * sizeof(**arr)) / sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
}

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
                if (powmod((seq_t)g, cand, (seq_t)p) == (seq_t)1)
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
            if (powmod((seq_t)g, cand, (seq_t)p) == (seq_t)1)
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
            if (cache_len >= cache_cap) {                 /* grow before storing */
                cache_cap = cache_cap ? 2 * cache_cap : 16;
                seq_t *grown_ = mem_resize(seq_t, cache, cache_cap);
                if (!grown_) abort();
                cache = grown_;
            }
            cache[cache_len++] = (seq_t)last_prime;
        }
    }

    return cache[(size_t)n - 1];
}
