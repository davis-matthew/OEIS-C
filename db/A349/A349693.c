/*
 * A349693  Dirichlet convolution of the ruler function (A001511) with itself.
 * https://oeis.org/A349693
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Dirichlet ∘(ruler, ruler) = Σ_{d|n}
 *            A001511(d)·A001511(n/d) * Multiplicative: a(1)=1, a(p^e)=e+1 for
 *            odd p, a(2^e)=C(e+3,3) * where C(n,k) is the binomial
 *            coefficient
 * Range:     u8 n <= 31, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 96 terms through n = 9754 (2026-11-01)
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"

/* Compute binomial coefficient C(e+3,3) for small e  (e≥0) */
static seq_t binom_cubic(idx_t e)
{
    seq_t a = e + 3, b = e + 2, c = e + 1;
    seq_t r = a * b;
    r *= c;
    r /= 6;          /* 3! = 6 */
    return r;
}

seq_t a(idx_t n)
{
    if (n == 1) return 1;          /* base case: a(1)=1 */

    /* Temporary arrays for prime factors and exponents. */
    /* 64 elements is more than enough for n ≤ 10¹⁸ (prime count < 12). */
    uint64_t primes[64], exps[64];
    size_t cnt;

    factor_u64(n, primes, exps, &cnt);

    seq_t result = 1;

    for (size_t i = 0; i < cnt; ++i) {
        idx_t e = exps[i];
        seq_t term = (primes[i] == 2) ? binom_cubic(e) : (e + 1);
        result *= term;
    }

    return result;
}
