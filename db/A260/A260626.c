/*
 * A260626  a(n) = gcd(m, 2^m-1) where m is the n-th nonprime positive
 *          integer.
 * https://oeis.org/A260626
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = gcd(m, 2^m-1) where m is the n-th nonprime integer
 * Range:     u8 n <= 603, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/gcd.h"
#include "../utils/mod_pow.h"

/* ---------- prime sieve helper ---------- */
static uint64_t *prime_list = NULL;   /* array of discovered primes   */
static size_t prime_count = 0;       /* number of primes stored     */
static size_t prime_cap   = 0;       /* allocated capacity          */

static void add_prime(uint64_t p)
{
    if (prime_count == prime_cap) {
        size_t new_cap = prime_cap ? prime_cap * 2 : 4;
        uint64_t *tmp = (uint64_t *)realloc(prime_list,
                                            new_cap * sizeof(uint64_t));
        prime_list = tmp;
        prime_cap   = new_cap;
    }
    prime_list[prime_count++] = p;
}

static bool is_prime_uint64(uint64_t num)
{
    if (num < 2) return false;
    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = prime_list[i];
        if (p * p > num) break;
        if (num % p == 0) return false;
    }
    return true;
}

/* ---------- nonprime value helper ---------- */
static idx_t *nonprime_vals = NULL;   /* cached nonprime integers   */
static size_t nonprime_count = 0;     /* number of entries          */
static size_t nonprime_cap   = 0;     /* allocated capacity         */
static idx_t  last_candidate = 0;     /* last integer tested        */

static void add_nonprime(idx_t val)
{
    if (nonprime_count == nonprime_cap) {
        size_t new_cap = nonprime_cap ? nonprime_cap * 2 : 8;
        idx_t *tmp = (idx_t *)realloc(nonprime_vals,
                                      new_cap * sizeof(idx_t));
        nonprime_vals = tmp;
        nonprime_cap   = new_cap;
    }
    nonprime_vals[nonprime_count++] = val;
}

/* ---------- sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n<1 */

    /* generate non‑prime numbers until we have at least n entries */
    while (nonprime_count < n) {
        ++last_candidate;
        if (is_prime_uint64((uint64_t)last_candidate)) {
            add_prime((uint64_t)last_candidate);
        } else {
            add_nonprime(last_candidate);
        }
    }

    /* m is the n-th nonprime integer */
    idx_t m_val = nonprime_vals[n - 1];
    seq_t m = (seq_t)m_val;

    /* compute 2^m mod m */
    seq_t pow_mod = mod_pow((seq_t)2, m_val, m);

    /* r = (2^m mod m) – 1; if pow_mod == 0 then r = m – 1 */
    seq_t r = (pow_mod == (seq_t)0) ? m - (seq_t)1 : pow_mod - (seq_t)1;

    /* return gcd(m, r) */
    return gcd(m, r);
}
