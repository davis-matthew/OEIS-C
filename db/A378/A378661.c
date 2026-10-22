/*
 * A378661  Characteristic function of A083211: a(n) = 1 if n is abundant and
 *          is not Zumkeller, otherwise 0.
 * https://oeis.org/A378661
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9605, u16 n <= 9605, u32 n <= 9605, u64 n <= 9605,
 *            u128 n <= 9605, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A378661:
   a(n) = 1 if n is abundant and not Zumkeller, otherwise 0.
   Computes using divisor sum (sigma) and a subset‑sum DP
   to test for Zumkeller property. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/factor_u64.h"   /* prime factorisation helper */

/* Generate all divisors from prime factorisation */
static void generate_divisors(const uint64_t *primes,
                              const uint64_t *exps,
                              size_t cnt,
                              uint64_t *divs,
                              size_t *out_cnt)
{
    size_t dcnt = 1;
    divs[0] = 1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p   = primes[i];
        uint64_t e   = exps[i];
        size_t cur   = dcnt;
        uint64_t p_pow = 1;

        for (uint64_t k = 1; k <= e; ++k) {
            p_pow *= p;
            for (size_t j = 0; j < cur; ++j)
                divs[dcnt++] = divs[j] * p_pow;
        }
    }
    *out_cnt = dcnt;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* undefined for n < 1, safe guard */

    uint64_t nn = (uint64_t)n;

    /* Factorise n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t   fac_cnt = 0;
    factor_u64(nn, primes, exps, &fac_cnt);

    /* Compute sigma(n) as unsigned long long to avoid overflow */
    unsigned long long usigma = 1;
    for (size_t i = 0; i < fac_cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        unsigned long long term = 1;
        unsigned long long p_pow = 1;
        for (uint64_t k = 1; k <= e; ++k) {
            p_pow *= (unsigned long long)p;
            term  += p_pow;
        }
        usigma *= term;
    }

    /* Abundance check: sigma(n) > 2*n */
    if (usigma <= 2ULL * nn)
        return 0;

    /* If sigma is odd → cannot be Zumkeller */
    if (usigma & 1ULL)
        return 1;

    /* Target sum for partition: sigma/2 */
    size_t target = (size_t)(usigma >> 1);

    /* Enumerate all divisors */
    uint64_t divs[512];   /* sufficient for n ≤ 10^4 */
    size_t dcnt = 0;
    generate_divisors(primes, exps, fac_cnt, divs, &dcnt);

    /* Subset‑sum DP for partition into two parts of equal sum */
    uint8_t *dp = (uint8_t *)calloc(target + 1, 1); /* dp[0] = 1 */
    dp[0] = 1;
    for (size_t i = 0; i < dcnt; ++i) {
        uint64_t d = divs[i];
        if (d > target)
            continue;
        size_t di = (size_t)d;
        for (size_t s = target; s >= di; --s) {
            if (dp[s - di])
                dp[s] = 1;
        }
    }
    bool is_zumkeller = dp[target] != 0;
    free(dp);

    /* Return 1 if abundant AND NOT Zumkeller, else 0 */
    return is_zumkeller ? 0 : 1;
}
