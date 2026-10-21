/*
 * A296092  Least number with the same prime signature as sigma(n)+1.
 * https://oeis.org/A296092
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 127, u16 n <= 9400, u32 n <= 9400, u64 n <= 9400,
 *            u128 n <= 9400, big any n
 * Helpers:   build_primes_upto, seq_pow, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS formula: a(n) = A046523(1 + A000203(n))
 *  where A000203(n) is the prime‑counting function σ₀(n).
 *  a(n) is the least integer having the same prime signature
 *  (multiset of exponents) as (σ(n)+1).  */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "../utils/sigma_of.h"            /* seq_t sigma_of(idx_t n);      */
#include "../utils/seq_pow.h"             /* seq_t seq_pow(seq_t base, idx_t exp); */
#include "../utils/build_primes_upto.h"   /* void build_primes_upto(idx_t limit,
                                          uint64_t **primes_out, size_t *cnt_out); */

/* First few primes – enough for all n up to at least 10000. */
static const uint64_t SMALL_PRIMES[] = {
    2, 3, 5, 7, 11, 13, 17, 19, 23, 29,
    31, 37, 41, 43, 47, 53, 59, 61, 67, 71,
    73, 79, 83, 89, 97
};
#define SMALL_PRIMES_CNT (sizeof(SMALL_PRIMES)/sizeof(SMALL_PRIMES[0]))

/* Cached primes up to a generous upper bound (sufficient for n ≤ 10000). */
static uint64_t *g_primes   = NULL;
static size_t    g_primes_cnt = 0;
static bool      g_primes_initialized = false;

/* Helper: Sort exponents descending (bubble sort works for the very small
 * size of the array).                                        */
static void sort_descending(uint64_t *v, size_t n)
{
    for (size_t i = 0; i < n; ++i)
        for (size_t j = i + 1; j < n; ++j)
            if (v[i] < v[j]) {
                uint64_t t = v[i];
                v[i] = v[j];
                v[j] = t;
            }
}

seq_t a(idx_t n)
{
    /* Ensure the global prime list is built only once. */
    if (!g_primes_initialized) {
        /* A safe upper bound: for n ≤ 10000, σ(n) < 17000, so sqrt ≈ 130.
           We use a larger limit for absolute safety. */
        const idx_t LIMIT = 100000;
        build_primes_upto(LIMIT, &g_primes, &g_primes_cnt);
        g_primes_initialized = true;
    }

    /* 1. Compute σ(n) + 1. */
    seq_t sigma = sigma_of(n);
    uint64_t val = (uint64_t)(sigma + 1);   /* fits in uint64_t for n ≤ 10000 */

    /* 2. Factorize val and collect exponents. */
    uint64_t exps[64];   /* more than enough for any small number */
    size_t   exps_cnt = 0;
    uint64_t tmp = val;

    for (size_t i = 0; i < g_primes_cnt && (uint64_t)g_primes[i] * g_primes[i] <= tmp; ++i) {
        if (tmp % g_primes[i] == 0) {
            uint64_t e = 0;
            while (tmp % g_primes[i] == 0) {
                tmp /= g_primes[i];
                ++e;
            }
            exps[exps_cnt++] = e;
        }
    }
    if (tmp > 1) {        /* remaining prime factor */
        exps[exps_cnt++] = 1;
    }

    /* 3. Sort exponents descending to match the smallest primes. */
    sort_descending(exps, exps_cnt);

    /* 4. Build the least number with the same prime signature. */
    seq_t result = 1;
    for (size_t i = 0; i < exps_cnt; ++i) {
        seq_t part = seq_pow((seq_t)SMALL_PRIMES[i], (idx_t)exps[i]);
        result *= part;
    }

    return result;
}
