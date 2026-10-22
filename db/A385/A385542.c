/*
 * A385542  The sum of the aliquot divisors of n that are powerful.
 * https://oeis.org/A385542
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A183097(n) - A112526(n) * n Equivalent: sum of powerful
 *            proper divisors of n (a powerful divisor is one in which every
 *            prime factor occurs with exponent ≥ 2)
 * Range:     u8 n <= 215, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n=0, guard against it */
    /* factorize n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(n, primes, exps, &cnt);

    seq_t sum = 1;
    bool powerful = true;

    for (size_t i = 0; i < cnt; ++i) {
        size_t e = (size_t)exps[i];
        seq_t partial = 1;           /* exponent 0 contributes 1 */
        if (e >= 2) {
            for (size_t f = 2; f <= e; ++f) {
                partial += pow_uint(primes[i], f);
            }
        } else {
            powerful = false;        /* exponent 1 → not powerful */
        }
        sum *= partial;
    }

    /* subtract n itself if it is powerful */
    if (powerful) {
        sum -= (seq_t)n;
    }
    return sum;
}
