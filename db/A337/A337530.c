/*
 * A337530  Number of ways that the divisors of 2n can be written as the sum
 *          of the squares of two other divisors of 2n (not necessarily
 *          distinct).
 * https://oeis.org/A337530
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{d1,d2,d3|2n, d1<=d2<=d3}[d1^2 + d2^2 = d3]
 * Range:     u8 n <= 95, u16 n <= 95, u32 n <= 95, u64 n <= 95, u128 n <= 95,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Utility for prime factorisation of a 64‑bit integer */
#include "../utils/factor_u64.h"

/* Comparator for uint64_t values used by qsort/bsearch */
static int cmp_uint64(const void *p1, const void *p2)
{
    uint64_t a = *(const uint64_t *)p1;
    uint64_t b = *(const uint64_t *)p2;
    return (a > b) - (a < b);
}

/* Recursive generation of all divisors of a number
   given its list of prime bases and exponents. */
static void gen_divs_rec(size_t pos, uint64_t cur,
                         const uint64_t *pr, const uint64_t *ex,
                         size_t pcnt,
                         uint64_t *divs, size_t *cnt)
{
    if (pos == pcnt) {
        divs[(*cnt)++] = cur;
        return;
    }
    uint64_t p = pr[pos];
    uint64_t v = cur;
    for (uint64_t e = 0; e <= ex[pos]; ++e) {
        gen_divs_rec(pos + 1, v, pr, ex, pcnt, divs, cnt);
        v *= p;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                    /* not defined, but harmless */

    /* 2*n fits in uint64_t because idx_t is uint64_t */
    uint64_t two_n = 2ULL * (uint64_t)n;

    /* Factorise two_n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t pcnt = 0;
    factor_u64(two_n, primes, exps, &pcnt);

    /* Generate all divisors of two_n */
    static uint64_t divs[65536];
    size_t divcnt = 0;
    gen_divs_rec(0, 1ULL, primes, exps, pcnt, divs, &divcnt);

    /* Sort divisors for binary search */
    qsort(divs, divcnt, sizeof(uint64_t), cmp_uint64);

    /* Count valid triples (d1 <= d2 <= d3) */
    uint64_t count = 0;
    for (size_t i = 0; i < divcnt; ++i) {
        uint64_t d1 = divs[i];
        for (size_t j = i; j < divcnt; ++j) {
            uint64_t d2 = divs[j];
            __uint128_t sumsq = (__uint128_t)d1 * d1 + (__uint128_t)d2 * d2;
            if (sumsq > UINT64_MAX) continue;  /* sum too large for any divisor */
            uint64_t target = (uint64_t)sumsq;
            if (bsearch(&target, divs, divcnt, sizeof(uint64_t), cmp_uint64))
                ++count;
        }
    }

    return (seq_t)count;
}
