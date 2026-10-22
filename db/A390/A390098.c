/*
 * A390098  Record denominators of (p1+1)/(p2+1), where p1 < p2 are
 *          consecutive primes.
 * https://oeis.org/A390098
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th record denominator of the reduced fraction
 *            (p₁+1)/(p₂+1) where p₁<p₂ are consecutive primes
 * Range:     u8 n <= 31, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* External linkage is required; no static keyword on the function name. */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/gcd.h"   /* provides seq_t gcd(seq_t, seq_t) */

/* Static state for generating primes and recording denominators. */
static uint64_t *prime_arr   = NULL;   /* array of generated primes */
static size_t   prime_cnt   = 0;
static size_t   prime_cap   = 0;
static uint64_t candidate   = 3;       /* next odd number to test for primality */
static uint64_t last_prime  = 0;       /* last prime processed (p₁) */

static seq_t   *record_arr   = NULL;   /* list of record denominators */
static size_t   record_cnt   = 0;
static size_t   record_cap   = 0;
static seq_t    max_den      = 0;      /* current maximum denominator */

/* Ensure prime array has room for at least one more element. */
static void ensure_prime_cap(void)
{
    if (prime_cnt >= prime_cap) {
        size_t ncap = prime_cap ? prime_cap * 2 : 32;
        uint64_t *tmp = (uint64_t *)realloc(prime_arr, ncap * sizeof(uint64_t));
        prime_arr = tmp;
        prime_cap = ncap;
    }
}

/* Ensure record array has room for at least one more element. */
static void ensure_record_cap(void)
{
    if (record_cnt >= record_cap) {
        size_t ncap = record_cap ? record_cap * 2 : 32;
        seq_t *tmp = (seq_t *)realloc(record_arr, ncap * sizeof(seq_t));
        record_arr = tmp;
        record_cap = ncap;
    }
}

/* Simple primality test using the primes that have already been generated. */
static bool is_prime(uint64_t n)
{
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_arr[i];
        if (p * p > n)
            break;
        if (n % p == 0)
            return false;
    }
    return true;
}

/* Generate the next prime after the current candidate. */
static uint64_t next_prime(void)
{
    for (;;) {
        uint64_t n = candidate;
        candidate += 2;                   /* skip even numbers */
        if (is_prime(n)) {
            ensure_prime_cap();
            prime_arr[prime_cnt++] = n;
            return n;
        }
    }
}

/* Generate records up to at least `max_needed` entries. */
static void generate_records(idx_t max_needed)
{
    /* Initialise prime list on first call. */
    if (prime_cnt == 0) {
        ensure_prime_cap();
        prime_arr[prime_cnt++] = 2;   /* the first prime */
        last_prime = 2;
        candidate = 3;
    }

    /* Produce consecutive primes and check the reduced fractions. */
    while (record_cnt < (size_t)max_needed) {
        uint64_t p2 = next_prime();
        uint64_t p1 = last_prime;

        seq_t a = (seq_t)(p1 + 1);
        seq_t b = (seq_t)(p2 + 1);
        seq_t g = gcd(a, b);
        seq_t denom = b / g;          /* denominator after reduction */

        if (denom > max_den) {
            ensure_record_cap();
            record_arr[record_cnt++] = denom;
            max_den = denom;
        }
        last_prime = p2;               /* move window to next pair */
    }
}

/* Public function required by the framework. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* undefined for offset 1 */

    static idx_t max_generated = 0;

    if (n > max_generated) {
        generate_records(n);
        max_generated = n;
    }
    return record_arr[n - 1];
}
