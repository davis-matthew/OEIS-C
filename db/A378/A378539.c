/*
 * A378539  Characteristic function for numbers that have Zumkeller divisors.
 * https://oeis.org/A378539
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n has a divisor that is a Zumkeller number, else 0
 *            (a(n) = [A378446(n) > 0])
 * Range:     u8 n <= 9566, u16 n <= 9566, u32 n <= 9566, u64 n <= 9566,
 *            u128 n <= 9566, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/factor_u64.h"

/* ------------------------------------------------------------------ */
/* Generate all divisors of m using its prime factorization.           */
/* ------------------------------------------------------------------ */
static uint64_t *divisors_of(uint64_t m, size_t *len_out)
{
    uint64_t primes[64], exps[64];
    size_t cnt = 0;
    factor_u64(m, primes, exps, &cnt);

    /* start with divisor 1 */
    size_t cap = 16;
    size_t len = 0;
    uint64_t *divs = (uint64_t *)malloc(cap * sizeof(uint64_t));
    if (!divs) {
        *len_out = 0;
        return NULL;
    }
    divs[0] = 1;
    len = 1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        /* precompute powers p^k for k = 1..e */
        uint64_t pow = 1;
        uint64_t *powers = (uint64_t *)malloc(e * sizeof(uint64_t));
        if (!powers) {
            free(divs);
            *len_out = 0;
            return NULL;
        }
        for (uint64_t k = 0; k < e; ++k) {
            pow = (uint64_t)((__int128)pow * p);
            powers[k] = pow;
        }
        /* for each existing divisor, multiply by each power */
        size_t old_len = len;
        for (size_t j = 0; j < old_len; ++j) {
            uint64_t base = divs[j];
            for (uint64_t k = 0; k < e; ++k) {
                uint64_t newdiv = (uint64_t)((__int128)base * powers[k]);
                if (len == cap) {
                    cap <<= 1;
                    uint64_t *tmp = (uint64_t *)realloc(divs, cap * sizeof(uint64_t));
                    if (!tmp) { free(divs); free(powers); *len_out = 0; return NULL; }
                    divs = tmp;
                }
                divs[len++] = newdiv;
            }
        }
        free(powers);
    }
    *len_out = len;
    return divs;
}

/* ------------------------------------------------------------------ */
/* Test if a positive integer m is a Zumkeller number.                 */
/* ------------------------------------------------------------------ */
static bool is_zumkeller(uint64_t m)
{
    /* m = 1 is not Zumkeller */
    if (m == 1) return false;

    uint64_t primes[64], exps[64];
    size_t cnt = 0;
    factor_u64(m, primes, exps, &cnt);

    /* compute sigma(m) with __int128 to avoid overflow */
    __int128 sigma = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        __int128 term = 1;
        __int128 pow = 1;
        for (uint64_t k = 1; k <= e; ++k) {
            pow *= p;
            term += pow;
        }
        sigma *= term;
    }

    /* sigma must be even */
    if ((sigma & 1) != 0) return false;

    uint64_t target = (uint64_t)(sigma >> 1);   /* sigma / 2 */

    /* generate all divisors of m */
    size_t d_len = 0;
    uint64_t *divs = divisors_of(m, &d_len);
    if (!divs) return false;                    /* allocation failure */

    /* subset sum DP */
    /* dp array of unsigned char to save memory */
    unsigned char *dp = (unsigned char *)calloc(target + 1, 1);
    if (!dp) { free(divs); return false; }
    dp[0] = 1;
    for (size_t i = 0; i < d_len; ++i) {
        uint64_t d = divs[i];
        if (d > target) continue;
        size_t d_us = (size_t)d;
        for (size_t s = target; s >= d_us; --s) {
            if (dp[s - d_us]) dp[s] = 1;
        }
        if (dp[target]) { free(dp); free(divs); return true; }
    }
    free(dp);
    free(divs);
    return false;
}

/* ------------------------------------------------------------------ */
/* Main sequence function                                              */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    /* obtain divisors of n */
    size_t dlen = 0;
    uint64_t *divs = divisors_of((uint64_t)n, &dlen);
    if (!divs) return (seq_t)0;      /* allocation error */

    for (size_t i = 0; i < dlen; ++i) {
        if (is_zumkeller(divs[i])) {
            free(divs);
            return (seq_t)1;
        }
    }
    free(divs);
    return (seq_t)0;
}
