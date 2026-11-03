/*
 * A326039  Largest square dividing the sum of divisors of n: a(n) =
 *          A008833(sigma(n)).
 * https://oeis.org/A326039
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest square divisor of σ(n) (sigma_of(n))
 * Range:     u8 n <= 169, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 112 terms through n = 9069 (2026-11-01)
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
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

/* Power with an unsigned exponent, result is seq_t */
static seq_t pow_seq_uint(seq_t base, uint64_t exp)
{
    seq_t res = 1;
    while (exp > 0) {
        if (exp & 1U)
            res *= base;
        base *= base;
        exp >>= 1U;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* Compute σ(n) */
    seq_t sigma = sigma_of(n);

    /* σ(n) fits in 64 bits for n ≤ 10 000, cast safely */
    uint64_t sig = (uint64_t)sigma;

    /* Factor σ(n) */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(sig, primes, exps, &cnt);

    /* Build the largest square divisor */
    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t exp = exps[i];
        uint64_t even_exp = (exp / 2U) * 2U;       /* floor(exp/2)*2 */
        if (even_exp > 0) {
            seq_t part = pow_seq_uint((seq_t)primes[i], even_exp);
            result *= part;
        }
    }
    return result;
}
