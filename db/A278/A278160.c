/*
 * A278160  Least number with the prime signature of ((n+1)^2 - 1).
 * https://oeis.org/A278160
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 263, u32 n <= 1024, u64 n <= 1024,
 *            u128 n <= 1024, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS formula: a(n) = least number with the same prime signature as n*(n+2)
   (i.e. as ((n+1)^2) - 1). The function works for any seq_t that can hold
   the result; for seq_t larger than 64 bits, the intermediate integer `m`
   must still fit in uint64_t (this holds for all test cases with n ≤ 10⁴). */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* first 20 primes – enough for any m ≤ 2^64 (max 16 distinct factors) */
static const uint64_t SMALL_PRIMES[20] = {
    2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,
    31ULL,37ULL,41ULL,43ULL,47ULL,53ULL,59ULL,61ULL,67ULL,71ULL
};
static const size_t SMALL_PRIMES_CNT = sizeof(SMALL_PRIMES) / sizeof(SMALL_PRIMES[0]);

/* comparison for qsort – ascending on unsigned long */
static int cmp_ulong(const void *a, const void *b)
{
    unsigned long aa = *(const unsigned long*)a;
    unsigned long bb = *(const unsigned long*)b;
    return (aa > bb) - (aa < bb);
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined for the given offset – any value allowed */

    /* m = n*(n+2)  (in seq_t to preserve arbitrary precision) */
    seq_t m = (seq_t)n * ((seq_t)n + (seq_t)2);

    /* --- factor m (assumed to fit in uint64_t) -------------------------------- */
    uint64_t m64 = (uint64_t)m;          /* truncation is safe for all tested n */
    unsigned long exps[64];
    size_t exps_cnt = 0;

    uint64_t tmp = m64;
    unsigned long e = 0;

    /* factor 2 */
    while ((tmp & 1ULL) == 0) {
        tmp >>= 1;
        ++e;
    }
    if (e > 0)
        exps[exps_cnt++] = e;

    /* odd factors */
    for (uint64_t p = 3; p * p <= tmp; p += 2) {
        if (tmp % p == 0) {
            e = 0;
            do {
                tmp /= p;
                ++e;
            } while (tmp % p == 0);
            exps[exps_cnt++] = e;
        }
    }
    if (tmp > 1) /* remaining prime factor > sqrt(original m) */
        exps[exps_cnt++] = 1;

    /* sort exponents descending */
    qsort(exps, exps_cnt, sizeof(unsigned long), cmp_ulong);
    for (size_t i = 0; i < exps_cnt / 2; ++i) {
        unsigned long t = exps[i];
        exps[i] = exps[exps_cnt - 1 - i];
        exps[exps_cnt - 1 - i] = t;
    }

    /* --- build the smallest number with that signature ------------------------ */
    seq_t result = 1;
    for (size_t i = 0; i < exps_cnt; ++i) {
        seq_t base = (seq_t)SMALL_PRIMES[i];
        for (unsigned long k = 0; k < exps[i]; ++k)
            result *= base;
    }

    return result;
}
