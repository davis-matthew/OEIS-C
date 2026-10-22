/*
 * A306247  Numbers k such that 2k - p is not a prime where p is any prime
 *          divisor of 4k^2 - 1.
 * https://oeis.org/A306247
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 43, u16 n <= 9942, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A306247 – Numbers k such that for every prime divisor p of 4k²–1,
    the value 2k – p is NOT prime.  Computed by incremental search
    generating k one by one and testing the condition.   */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "../utils/is_prime_big.h"
#include "../utils/factor_u64.h"

#define MAX_TERMS 20000           /* enough for n ≤ 10000 */
#define MAX_FACTORS 64            /* limit for distinct prime factors of 2k±1 */

/*---------------------------------------------------------------------------*/
/*  Test whether a given unsigned k satisfies the defining condition.       */
static bool is_term(uint64_t k)
{
    uint64_t primes[MAX_FACTORS];
    uint64_t exps[MAX_FACTORS];
    size_t   cnt = 0;
    size_t   uniq = 0;
    uint64_t uniq_primes[MAX_FACTORS];
    uint64_t a = 2ULL * k - 1ULL;
    uint64_t b = 2ULL * k + 1ULL;

    /* factor 2k-1 */
    factor_u64(a, primes, exps, &cnt);
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        bool found = false;
        for (size_t j = 0; j < uniq; ++j)
            if (uniq_primes[j] == p) { found = true; break; }
        if (!found) uniq_primes[uniq++] = p;
    }

    /* factor 2k+1 */
    factor_u64(b, primes, exps, &cnt);
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        bool found = false;
        for (size_t j = 0; j < uniq; ++j)
            if (uniq_primes[j] == p) { found = true; break; }
        if (!found) uniq_primes[uniq++] = p;
    }

    /* For every distinct prime divisor check 2k - p */
    for (size_t i = 0; i < uniq; ++i) {
        uint64_t p  = uniq_primes[i];
        int64_t  diff = (int64_t)(2ULL * k) - (int64_t)p;
        if (diff >= 2) {
            /* diff fits into seq_t because diff ≤ 2k-2 and k is small */
            if (is_prime_big((seq_t)diff))
                return false;
        }
    }
    return true;  /* passes all tests */
}

/*---------------------------------------------------------------------------*/
/*  Generate terms of the sequence up to the requested index n.              */
static seq_t terms[MAX_TERMS];
static size_t terms_len = 0;      /* number of stored terms   */
static uint64_t next_k = 1;       /* next k to test           */

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for offset 1 */

    /* Ensure we have at least n terms in the table */
    while (terms_len < (size_t)n) {
        if (terms_len >= MAX_TERMS) {
            /* safety: should never happen for requested limits */
            return 0;
        }
        uint64_t k = next_k;
        if (is_term(k)) {
            terms[terms_len++] = (seq_t)k;
        }
        ++next_k;
    }

    return terms[n - 1];   /* 1‑based indexing */
}
