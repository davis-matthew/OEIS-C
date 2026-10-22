/*
 * A343301  Numbers k such that 6*k+1 through 6*k+5 are all deficient (in
 *          A005100).
 * https://oeis.org/A343301
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 133, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A343301 – Numbers k such that 6*k+1 through 6*k+5 are all deficient.
    The function builds the terms on demand and caches them.                */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"

/* Integer square‑root for uint64_t (binary search). */
static uint64_t isqrt_uint64(uint64_t n)
{
    if (n < 2) return n;
    uint64_t x = n >> 1;
    while (1) {
        uint64_t y = (x + n / x) >> 1;
        if (y >= x) return x;
        x = y;
    }
}

/* Prime list (generated up to prime_gen_limit). */
static uint64_t *prime_list = NULL;
static size_t    prime_cnt  = 0;
static uint64_t  prime_gen_limit = 0;

/* Ensure we have primes up to at least 'limit'. */
static void ensure_primes_upto(uint64_t limit)
{
    if (limit <= prime_gen_limit) return;
    free(prime_list);
    prime_list = NULL;
    prime_cnt  = 0;
    build_primes_upto(limit, &prime_list, &prime_cnt);
    prime_gen_limit = limit;
}

/* Test whether n is deficient (sum of proper divisors < n). */
static bool is_deficient(uint64_t n)
{
    if (n <= 1) return true;  /* 1 is deficient */

    uint64_t m = n;
    __int128 sigma = 1;         /* product of (p^(e+1)-1)/(p-1) */

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        if (p * p > m) break;
        if (m % p == 0) {
            __int128 term = 1;
            uint64_t pow   = 1;
            do {
                m /= p;
                pow *= p;
                term += pow;
            } while (m % p == 0);
            sigma *= term;
            if (sigma >= (__int128)2 * n) return false;
        }
    }
    if (m > 1) {                 /* remaining prime factor */
        sigma *= (1 + (__int128)m);
        if (sigma >= (__int128)2 * n) return false;
    }
    return sigma < (__int128)2 * n;
}

/* Cache of sequence terms. */
static seq_t *terms   = NULL;
static size_t  terms_len = 0;
static size_t  terms_cap = 0;
static uint64_t next_k  = 0;                /* k value to test next */

/* Append a value to the cache (grow array if needed). */
static void add_term(seq_t val)
{
    if (terms_len == terms_cap) {
        terms_cap = (terms_cap == 0) ? 8 : terms_cap << 1;
        terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
        if (!terms) abort();
    }
    terms[terms_len++] = val;
}

/*  A343301 sequence generator. */
seq_t a(idx_t n)
{
    /* Grow the cache until we have at least n terms. */
    while (terms_len < (size_t)n) {
        uint64_t k = next_k++;

        uint64_t max_m = 6 * k + 5;
        uint64_t needed = isqrt_uint64(max_m);
        ensure_primes_upto(needed);

        bool ok = true;
        for (int off = 1; off <= 5; ++off) {
            if (!is_deficient(6 * k + off)) {
                ok = false;
                break;
            }
        }
        if (ok) add_term((seq_t)k);
    }
    return terms[(size_t)n - 1];
}
