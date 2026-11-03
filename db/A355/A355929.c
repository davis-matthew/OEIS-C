/*
 * A355929  Difference between the squarefree part of the sum of divisors of n
 *          and the squarefree part of n.
 * https://oeis.org/A355929
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = squarefree_part(sigma(n)) - squarefree_part(n) where
 *            squarefree_part(x) = x divided by the largest square dividing x
 *            (i.e. product of the primes of x to odd exponents)
 * Range:     i8 n <= 59, i16 n <= 11519, i32 n <= 20000, i64 n <= 20000,
 *            i128 n <= 20000, big any n
 * Validated: standard, pass, 92 terms through n = 9645 (2026-11-01)
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

static seq_t squarefree_part_uint64(uint64_t n)
{
    if (n == 0)
        return 0;
    /* Factor n (n <= 2^64). 64 primes is enough for any uint64_t. */
    uint64_t pr[64];
    uint64_t ex[64];
    size_t cnt = 0;
    factor_u64(n, pr, ex, &cnt);

    seq_t res = 1;
    for (size_t i = 0; i < cnt; ++i) {
        if ((ex[i] & 1ULL) != 0)
            res *= (seq_t)pr[i];
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;   /* undefined for n = 0, but return 0 for safety */

    /* Compute squarefree part of n */
    seq_t sf_n = squarefree_part_uint64((uint64_t)n);

    /* Sum of divisors sigma(n) */
    seq_t sigma = sigma_of(n);

    /* sigma fits in uint64_t for n up to 10^4 (and for typical tests) */
    uint64_t sigma_u64 = (uint64_t)sigma;

    /* Compute squarefree part of sigma(n) */
    seq_t sf_sigma = squarefree_part_uint64(sigma_u64);

    return sf_sigma - sf_n;
}
