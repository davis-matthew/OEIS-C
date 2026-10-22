/*
 * A380819  Triangle read by rows where row n lists "weak" divisors d | n
 *          (i.e., d in A052485) such that rad(d)^2 does not divide d, where
 *          rad = A007947.
 * https://oeis.org/A380819
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   not stated
 * Range:     u8 n <= 1004, u16 n <= 8995, u32 n <= 8995, u64 n <= 8995,
 *            u128 n <= 8995, big any n
 * Helpers:   genWeakRec
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle read by rows of weak divisors of n (offset 2) */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/genWeakRec.h"

/* factor a number into primes and exponents */
static void factor_uint64(uint64_t n, uint64_t *pr, int *exp, size_t *cnt) {
    size_t c = 0;
    for (uint64_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            int e = 0;
            while (n % p == 0) {
                n /= p;
                ++e;
            }
            pr[c] = p;
            exp[c] = e;
            ++c;
        }
    }
    if (n > 1) {
        pr[c] = n;
        exp[c] = 1;
        ++c;
    }
    *cnt = c;
}

/* comparator for qsort */
static int cmp_uint64(const void *a, const void *b) {
    uint64_t va = *(const uint64_t *)a;
    uint64_t vb = *(const uint64_t *)b;
    return (va < vb) ? -1 : (va > vb) ? 1 : 0;
}

/* sequence function */
seq_t a(idx_t n) {
    if (n < 2) return (seq_t)0; /* undefined below offset */

    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static uint64_t next_row = 2;            /* next row to process */

    size_t target = (size_t)(n - 2);          /* zero‑based index */

    /* extend cache if necessary */
    if (target < cache_len) return cache[target];

    if (!cache) {
        cache_cap = 256;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    } else if (cache_len + 1024 > cache_cap) {
        cache_cap *= 2;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }

    while (cache_len <= target) {
        /* factor current row */
        uint64_t primes[64];
        int exps[64];
        size_t pfCnt;
        factor_uint64(next_row, primes, exps, &pfCnt);

        /* total divisor count */
        uint64_t totalDiv = 1;
        for (size_t i = 0; i < pfCnt; ++i) totalDiv *= (uint64_t)(exps[i] + 1);

        /* powerful divisors (including 1) */
        uint64_t powerful_incl1 = 1;
        for (size_t i = 0; i < pfCnt; ++i)
            if (exps[i] >= 2) powerful_incl1 *= (uint64_t)(exps[i]);

        uint64_t powerful = powerful_incl1 - 1;       /* exclude 1   */
        uint64_t weakCount = totalDiv - powerful - 1; /* exclude 1   */

        if (weakCount > 0) {
            uint64_t *divs = (uint64_t *)malloc(weakCount * sizeof(uint64_t));
            size_t cnt = 0;
            genWeakRec(primes, exps, pfCnt, 0, 1, false, divs, &cnt);
            qsort(divs, weakCount, sizeof(uint64_t), cmp_uint64);
            for (size_t i = 0; i < weakCount; ++i) {
                if (cache_len >= cache_cap) {
                    cache_cap *= 2;
                    cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                }
                cache[cache_len++] = (seq_t)divs[i];
            }
            free(divs);
        }
        ++next_row;
    }

    return cache[target];
}
