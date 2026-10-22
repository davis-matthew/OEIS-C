/*
 * A326039  Largest square dividing the sum of divisors of n: a(n) =
 *          A008833(sigma(n)).
 * https://oeis.org/A326039
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest square divisor of σ(n) (sigma_of(n))
 * Range:     u8 n <= 169, u16 n <= 9069, u32 n <= 9069, u64 n <= 9069,
 *            u128 n <= 9069, big any n
 * Helpers:   factor_u64, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/sigma_of.h"
#include "../utils/factor_u64.h"

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
