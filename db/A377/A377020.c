/*
 * A377020  Numbers whose prime factorization has exponents that are all
 *          numbers of the form m*k!, where 1 <= m <= k.
 * https://oeis.org/A377020
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 226, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A377020:
   Numbers whose prime factorisation has exponents all of the
   form m*k! with 1 ≤ m ≤ k.  a(1)=1. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ---------- Small‑table of allowed exponents ----------
   Exponents are 1 or of the form m*k! for 1≤m≤k, k≥1. */
static bool allowed_exp[257];
static bool allowed_init = false;
static void init_allowed_exponents(void)
{
    if (allowed_init) return;
    allowed_init = true;
    for (int i = 0; i <= 256; ++i) allowed_exp[i] = false;

    int fact = 1;
    for (int k = 1; k <= 10; ++k) {          /* 10! > 256 */
        fact *= k;
        if (fact > 256) break;
        for (int m = 1; m <= k; ++m) {
            int e = m * fact;
            if (e <= 256) allowed_exp[e] = true;
        }
    }
    allowed_exp[1] = true;                    /* the exponent 1 is always allowed */
}

/* ---------- Prime sieve ----------
   Generates all primes up to a fixed limit.  The limit is small enough
   to cover sqrt of any number we encounter while searching the first
   several thousand terms. */
static uint32_t *prime_list = NULL;   /* primes in ascending order */
static size_t     prime_cnt   = 0;

static void init_prime_list(void)
{
    if (prime_cnt > 0) return;                       /* already built */

    const size_t LIMIT = 100000;                     /* > sqrt of all candidates we will hit */
    bool *is_composite = (bool *)calloc(LIMIT + 1, sizeof(bool));
    for (size_t p = 2; p * p <= LIMIT; ++p) {
        if (!is_composite[p]) {
            for (size_t q = p * p; q <= LIMIT; q += p)
                is_composite[q] = true;
        }
    }

    /* Count primes and allocate array */
    size_t cnt = 0;
    for (size_t i = 2; i <= LIMIT; ++i)
        if (!is_composite[i]) ++cnt;
    prime_list = (uint32_t *)malloc(cnt * sizeof(uint32_t));
    prime_cnt = cnt;

    /* Store primes */
    size_t idx = 0;
    for (size_t i = 2; i <= LIMIT; ++i)
        if (!is_composite[i]) prime_list[idx++] = (uint32_t)i;

    free(is_composite);
}

/* ---------- Test whether *x* has only allowed exponents ----------
   Uses trial division over the pre‑computed prime list. */
static bool exponents_allowed(uint64_t x)
{
    if (x <= 1) return true;                         /* 1 has no prime factors */

    uint64_t temp = x;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        if (p * p > temp) break;
        if (temp % p == 0) {
            int exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            if (exp > 256 || !allowed_exp[exp]) return false;
        }
    }
    /* Any remaining factor is prime with exponent 1 – always allowed */
    return true;
}

/* ---------- Sequence function ------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* offset 1: a(1)=1 */

    init_prime_list();
    init_allowed_exponents();

    uint64_t candidate = 1;                /* 1 is the first term */
    size_t  count      = 0;                /* how many terms found so far */

    while (count < n) {
        if (exponents_allowed(candidate)) {
            ++count;
            if (count == n) return (seq_t)candidate;
        }
        ++candidate;
    }

    /* Should never reach here */
    return 0;
}
