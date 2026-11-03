/*
 * A263676  Numbers that are both interprime and oblong.
 * https://oeis.org/A263676
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th number that is both an oblong number m*(m+1) and
 *            an interprime (average of two consecutive primes)
 * Range:     u8 n <= 7, u16 n <= 61, u32 n <= 5552, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 64 terms through n = 9339 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <limits.h>
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

/* -----------------  Generic primality test ------------------------ */
static bool isPrimeGeneric(seq_t n)
{
    /* all seq_t are unsigned; use the smallest type that fits */
    if ((seq_t)(ULLONG_MAX) >= n) {
        return is_prime_u64((uint64_t)n);
    }
#if defined(__SIZEOF_INT128__)
    return is_prime_u128((__uint128_t)n);
#else
    /* fallback: if seq_t > 64‑bit but no 128‑bit support,
       order the test as simple trial division up to sqrt */
    seq_t limit = 0;
    for (seq_t i = 2; i*i <= n; ++i) {
        if (n % i == 0) return false;
    }
    return true;
#endif
}

/* Greatest prime <= n (n >= 1). */
static seq_t prev_prime_leq(seq_t n)
{
    if (n <= 2)
        return 2;

    /* Make odd if even. */
    if ((n & 1U) == 0)
        n -= 1;

    while (n >= 2) {
        if (isPrimeGeneric(n))
            return n;
        n -= 2;
    }
    return 2;  /* should not reach */
}

/* Smallest prime > n (n >= 1). */
static seq_t next_prime_gt(seq_t n)
{
    seq_t candidate = n + 1;
    if (candidate <= 2)
        return 2;
    if ((candidate & 1U) == 0)
        candidate += 1;
    while (true) {
        if (isPrimeGeneric(candidate))
            return candidate;
        candidate += 2;
    }
}

/* External linkage as required */
seq_t a(idx_t n)
{
    if (n == 0)          /* not used but defined for safety */
        return 0;

    /* Static cache of found terms. */
    static seq_t *terms      = NULL;
    static size_t terms_len  = 0;
    static size_t terms_cap  = 0;
    static idx_t  last_m     = 1;   /* next m to examine */

    if (terms == NULL) {          /* first-time initialization */
        terms_cap = 64;
        terms     = mem_new(seq_t, terms_cap);
    }

    while (terms_len < n) {
        /* Next candidate m */
        ++last_m;
        seq_t m = (seq_t)last_m;
        seq_t B = m * (m + 1);          /* oblong m*(m+1) */

        /* p: largest prime < B */
        seq_t p = prev_prime_leq(B - 1);
        /* q: smallest prime > p */
        seq_t q = next_prime_gt(p);

        /* Check that p and q are consecutive and average to B */
        seq_t sum_pq = p + q;
        seq_t double_B = (seq_t)2 * B;
        if (sum_pq == double_B) {
            /* Store B */
            if (terms_len == terms_cap) {
                terms_cap *= 2;
                terms = mem_resize(seq_t, terms, terms_cap);
            }
            terms[terms_len++] = B;
        }
    }

    return terms[n - 1];
}
