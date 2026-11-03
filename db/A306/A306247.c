/*
 * A306247  Numbers k such that 2k - p is not a prime where p is any prime
 *          divisor of 4k^2 - 1.
 * https://oeis.org/A306247
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 43, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 81 terms through n = 9943 (2026-11-03)
 * Helpers:   factor_u64, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A306247 – Numbers k such that for every prime divisor p of 4k²–1,
    the value 2k – p is NOT prime.  Computed by incremental search
    generating k one by one and testing the condition.   */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

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
