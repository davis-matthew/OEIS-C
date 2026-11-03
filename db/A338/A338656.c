/*
 * A338656  a(n) = Sum_{d|n} mu(d) * binomial(d + n/d - 2, d-1).
 * https://oeis.org/A338656
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, look
 * Formula:   a(n) = Sum_{d|n} mu(d) * binomial(d + n/d - 2, d-1)
 * Range:     i8 n <= 32, i16 n <= 89, i32 n <= 314, i64 n <= 1146,
 *            i128 n <= 4415, big any n
 * Validated: standard, pass, 82 terms through n = 9746 (2026-11-01)
 * Helpers:   binomial, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/factor_u64.h"

/* Möbius function for the divisor in the loop */
static inline int mu_of(idx_t d) {
    int n = (int)d;
    int sign = 1;
    int i = 2;
    while ((int64_t)i * i <= n) {
        if (n % i == 0) {
            int cnt = 0;
            while (n % i == 0) {
                n /= i;
                ++cnt;
                if (cnt > 1) return 0;        /* square factor */
            }
            sign = -sign;
        }
        ++i;
    }
    if (n > 1) sign = -sign;
    return sign;
}
#include <stdlib.h>
#include <stddef.h>

/* Compute A338656 */
seq_t a(idx_t n)
{
    if (n == 1)
        return 1;   /* early return for the trivial case */

    /* Factor n using the provided routine */
    uint64_t pr[64];
    uint64_t ex[64];
    size_t pcnt = 0;
    factor_u64(n, pr, ex, &pcnt);

    /* Calculate the number of divisors */
    size_t numDivs = 1;
    for (size_t i = 0; i < pcnt; ++i)
        numDivs *= (size_t)(ex[i] + 1);

    /* Generate all divisors */
    idx_t *divisors = (idx_t *)malloc(numDivs * sizeof(idx_t));
    if (!divisors)
        return 0;   /* fallback on allocation failure */

    divisors[0] = 1;
    size_t curSize = 1;

    for (size_t i = 0; i < pcnt; ++i) {
        uint64_t p   = pr[i];
        uint64_t e   = ex[i];
        size_t oldSize = curSize;
        for (size_t j = 0; j < oldSize; ++j) {
            idx_t base = divisors[j];
            idx_t mul  = 1;
            for (uint64_t k = 1; k <= e; ++k) {
                mul *= (idx_t)p;
                divisors[curSize++] = base * mul;
            }
        }
    }

    /* Summation over all divisors */
    seq_t sum = 0;
    for (size_t i = 0; i < numDivs; ++i) {
        idx_t d  = divisors[i];
        int mu_d = mu_of(d);
        if (mu_d == 0)
            continue;

        idx_t nd = n / d;
        idx_t a_val = d + nd - 2;
        idx_t b_val = d - 1;
        seq_t bin = binomial(a_val, b_val);
        if (mu_d > 0)
            sum += bin;
        else
            sum -= bin;
    }

    free(divisors);
    return sum;
}
