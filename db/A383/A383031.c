/*
 * A383031  Indices of the squarefree terms in the sequence of exponentially
 *          odd numbers.
 * https://oeis.org/A383031
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 218, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   append_candidate, build_primes_upto, int_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = index of the nth squarefree term in the exponentially odd sequence */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#include "../utils/build_primes_upto.h"
#include "../utils/int_sqrt.h"
#include "../utils/append_candidate.h"

/* static storage */
static uint64_t *primes = NULL;      /* prime table */
static size_t prime_cnt = 0;
static idx_t   max_prime_limit = 0;

static seq_t  *indices = NULL;       /* collected indices */
static size_t indices_len = 0;
static size_t indices_cap = 0;

static uint64_t next_candidate = 1;   /* current natural number to test */
static idx_t    cur_ord      = 0;    /* ordinal among exponentially odd numbers */

/* ensure we have primes up to 'limit' */
static void ensure_primes_up_to(idx_t limit) {
    if (limit <= max_prime_limit) return;
    if (primes != NULL) free(primes);
    build_primes_upto(limit, &primes, &prime_cnt);
    max_prime_limit = limit;
}

/* factor n and set flags for exponential‑oddness and square‑freeness */
static void factor_and_check(uint64_t n, bool *out_expOdd, bool *out_sqfree) {
    if (n == 1) {
        *out_expOdd = true;
        *out_sqfree = true;
        return;
    }
    bool expOdd = true, sqfree = true;

    idx_t limit = int_sqrt(n);
    if (limit > max_prime_limit) ensure_primes_up_to(limit);

    for (size_t i = 0; i < prime_cnt && primes[i] <= limit; ++i) {
        uint64_t p = primes[i];
        if (n % p == 0) {
            int exp = 0;
            do { n /= p; exp++; } while (n % p == 0);
            if ((exp & 1) == 0) expOdd = false;   /* even exponent found */
            if (exp != 1) sqfree = false;         /* exponent > 1 found */
        }
    }
    /* any remaining factor has exponent 1, which is odd and square‑free */
    *out_expOdd = expOdd;
    *out_sqfree = sqfree;
}

/* main sequence function */
seq_t a(idx_t n) {
    if (indices_len >= n) return indices[n - 1];

    if (primes == NULL) ensure_primes_up_to(2);   /* initial prime table */

    while (indices_len < n) {
        bool expOdd, sqfree;
        factor_and_check(next_candidate, &expOdd, &sqfree);

        if (expOdd) {
            ++cur_ord;
            if (sqfree) {
                append_candidate(&indices, &indices_len, &indices_cap,
                                 (seq_t)cur_ord);
            }
        }
        ++next_candidate;
    }
    return indices[n - 1];
}
