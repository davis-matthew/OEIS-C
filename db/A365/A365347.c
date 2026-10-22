/*
 * A365347  The sum of divisors of the smallest number whose square is
 *          divisible by n.
 * https://oeis.org/A365347
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = σ( A019554(n) ), multiplicative: a(p^e) = (p^(⌈e/2⌉+1) –
 *            1)/(p – 1) (sum of divisors of p^⌈e/2⌉)
 * Range:     u8 n <= 16, u16 n <= 3323, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* n >= 1; for n == 1 the loop is skipped and result stays 1 */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t  cnt = 0;

    /* factorize n */
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;

    for (size_t i = 0; i < cnt; ++i) {
        seq_t p   = (seq_t)primes[i];
        seq_t e   = (seq_t)exps[i];
        /* ceil(e/2) = (e + 1) / 2 */
        seq_t ceil_half = (e + 1) / 2;
        /* exponent in numerator of σ(p^ceil_half) */
        seq_t exp_num = ceil_half + 1;

        seq_t pow_val = big_pow(p, exp_num);
        seq_t numerator   = pow_val - 1;
        seq_t denominator = p - 1;
        seq_t term = numerator / denominator;
        result *= term;
    }

    return result;
}
