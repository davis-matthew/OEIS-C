/*
 * A358215  Numbers k for which there is no prime p such that p^p divides the
 *          arithmetic derivative of k, A003415(k).
 * https://oeis.org/A358215
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 150, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the k-th member of OEIS A358215.
   The sequence consists of those integers k ≥ 2 for which no prime p
   satisfies p^p | A003415(k) (the arithmetic derivative of k).
   The function records candidates in the order they are discovered,
   so the result is independent of call order. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* ------------------------------------------------------------------ */
/* Prime sieve up to a fixed upper bound.  Adjust the bound if needed. */
/* ------------------------------------------------------------------ */
#define PRIME_LIMIT 2000000ULL

static uint64_t *prime_list = NULL;
static size_t prime_count   = 0;
static bool primes_inited   = false;

static void init_primes(void)
{
    if (primes_inited) return;

    bool *is_prime = (bool *)calloc(PRIME_LIMIT + 1, sizeof(bool));
    if (!is_prime) abort();

    for (uint64_t i = 2; i <= PRIME_LIMIT; ++i) is_prime[i] = true;
    for (uint64_t i = 2; i * i <= PRIME_LIMIT; ++i) {
        if (is_prime[i]) {
            for (uint64_t j = i * i; j <= PRIME_LIMIT; j += i)
                is_prime[j] = false;
        }
    }
    prime_list = (uint64_t *)malloc((PRIME_LIMIT / 10) * sizeof(uint64_t));
    if (!prime_list) abort();

    for (uint64_t i = 2; i <= PRIME_LIMIT; ++i)
        if (is_prime[i]) prime_list[prime_count++] = i;
    free(is_prime);
    primes_inited = true;
}

/* ------------------------------------------------------------------ */
/* Arithmetic derivative of n (n ≥ 2).  Uses trial division with the
   generated prime list.                                            */
/* ------------------------------------------------------------------ */
static seq_t derivative_of(uint64_t n)
{
    seq_t d = 0;
    uint64_t m = n;
    for (size_t idx = 0; idx < prime_count &&
                       prime_list[idx] <= m / prime_list[idx]; ++idx) {
        uint64_t p = prime_list[idx];
        if (m % p == 0) {
            uint64_t exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
            }
            d += (seq_t)exp * (seq_t)(n / p);
        }
    }
    if (m > 1) {                       /* remaining prime factor */
        d += (seq_t)(n / m);
    }
    return d;
}

/* ------------------------------------------------------------------ */
/* Test whether d has a divisor p^p for any prime p.  Only primes
   with p^p ≤ d are considered.  Overflows in p^p are avoided.       */
/* ------------------------------------------------------------------ */
static bool has_prime_power_divisor(seq_t d)
{
    for (size_t idx = 0; idx < prime_count; ++idx) {
        uint64_t p = prime_list[idx];
        seq_t p_pow = 1;
        for (uint64_t j = 0; j < p; ++j) {
            if (p_pow > d / p) {          /* would overflow or exceed d */
                p_pow = d + 1;
                break;
            }
            p_pow *= p;
        }
        if (p_pow > d) break;             /* further primes give larger powers */
        if (d % p_pow == 0) return true;
    }
    return false;
}

/* ------------------------------------------------------------------ */
/* Storage for discovered sequence terms.  Dynamically grows.          */
/* ------------------------------------------------------------------ */
static seq_t *terms      = NULL;
static size_t terms_len  = 0;
static size_t terms_cap  = 0;
static uint64_t next_k   = 2;   /* next integer to test for membership */

/* ------------------------------------------------------------------ */
/* Return the n-th member of the sequence (n ≥ 1).                       */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* undefined for 0 or negative indices */

    init_primes();

    /* Ensure capacity for the terms array. */
    if (terms_cap == 0) {
        terms_cap = 16;
        terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
        if (!terms) abort();
    }

    /* Populate the cache until we have at least n terms. */
    while (terms_len < (size_t)n) {
        seq_t d = derivative_of(next_k);
        if (!has_prime_power_divisor(d)) {
            if (terms_len == terms_cap) {
                terms_cap *= 2;
                terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
                if (!terms) abort();
            }
            terms[terms_len++] = (seq_t)next_k;
        }
        ++next_k;
    }

    return terms[n - 1];
}
