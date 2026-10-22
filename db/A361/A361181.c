/*
 * A361181  Numbers such that both sum and product of the prime factors
 *          (without multiplicity) are palindromic.
 * https://oeis.org/A361181
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th integer whose distinct prime factors have both a
 *            palindromic sum and a palindromic product (in base 10)
 * Range:     u8 n <= 38, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   _floor_isqrt_u64, is_palindrome, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation keeps a static array of accepted terms and a
   static list of primes.  For each new integer it factors it with the
   current prime list (expanding it if necessary) and checks the
   palindrome condition.  The arrays are grown with realloc, cast in a way
   that is valid in both C and C++. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Utility functions from the OEIS utilities.  The serial order of these
   includes does not matter, but the names must match those provided by
   the build system. */
#include "../utils/is_prime.h"
#include "../utils/is_palindrome.h"
#include "../utils/_floor_isqrt_u64.h"

/* ----- Static data ---- */
static seq_t    *terms   = NULL;   /* cache of accepted terms          */
static size_t   terms_len = 0, terms_cap = 0;
static uint64_t next_num  = 2;     /* next integer to test            */

static uint64_t *primes   = NULL;   /* list of primes                   */
static size_t   primes_len = 0, primes_cap = 0;

/* ----- Helper to grow the terms array ----- */
static void grow_terms(size_t needed)
{
    if (needed <= terms_cap)
        return;
    size_t new_cap = terms_cap ? terms_cap << 1 : 16;
    while (new_cap < needed)
        new_cap <<= 1;
    /* Cast is valid in both C and C++ */
    seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    if (!tmp) abort();                 /* allocation failed */
    terms = tmp;
    terms_cap = new_cap;
}

/* ----- Helper to grow the primes array ----- */
static void grow_primes(size_t needed)
{
    if (needed <= primes_cap)
        return;
    size_t new_cap = primes_cap ? primes_cap << 1 : 16;
    while (new_cap < needed)
        new_cap <<= 1;
    /* Cast is valid in both C and C++ */
    uint64_t *tmp = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
    if (!tmp) abort();
    primes = tmp;
    primes_cap = new_cap;
}

/* ----- Append a prime to the list ----- */
static void add_prime(uint64_t p)
{
    grow_primes(primes_len + 1);
    primes[primes_len++] = p;
}

/* ----- Generate primes up to at least 'limit' ----- */
static void generate_primes_up_to(uint64_t limit)
{
    if (primes_len == 0)
        add_prime(2);

    while (primes[primes_len - 1] < limit) {
        uint64_t cand = primes[primes_len - 1] + 1;
        for (;;) {
            if (is_prime((idx_t)cand)) {   /* cast to idx_t is safe: cand < 2^64 */
                add_prime(cand);
                break;
            }
            ++cand;
        }
    }
}

/* ----- Main sequence function ----- */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;      /* undefined for n=0, but return 0 for safety */

    /* Expand cache until it holds n terms */
    while (terms_len < (size_t)n) {
        uint64_t temp   = next_num;   /* value to factor */
        seq_t  sum      = 0;
        seq_t  prod     = 1;

        /* Ensure prime list covers sqrt(next_num) */
        generate_primes_up_to(_floor_isqrt_u64((idx_t)next_num));

        /* Factorize using the prime list */
        for (size_t i = 0; i < primes_len; ++i) {
            uint64_t p = primes[i];
            if (p * p > temp)
                break;
            if (temp % p == 0) {
                sum += (seq_t)p;
                prod *= (seq_t)p;
                while (temp % p == 0)
                    temp /= p;
            }
        }
        if (temp > 1) {           /* remaining prime factor */
            sum += (seq_t)temp;
            prod *= (seq_t)temp;
        }

        /* Check the palindrome condition */
        if (is_palindrome(sum, 10) && is_palindrome(prod, 10)) {
            grow_terms(terms_len + 1);
            terms[terms_len++] = (seq_t)next_num;
        }
        ++next_num;
    }

    return terms[n - 1];
}
