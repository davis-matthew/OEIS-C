/*
 * A341998  Arithmetic derivative of n divided by its largest squarefree
 *          divisor: a(n) = A003557(A003415(n)).
 * https://oeis.org/A341998
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A003557(A003415(n)) i.e. a(n) = arithmetic_derivative(n)
 *            / rad(arithmetic_derivative(n)) where rad(m) is the largest
 *            square‑free divisor of m (product of its distinct prime factors)
 * Range:     u8 n <= 255, u16 n <= 9928, u32 n <= 9928, u64 n <= 9928,
 *            u128 n <= 9928, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* sequence is defined for n >= 2; for smaller n return 0 */
    if (n < 2) return 0;

    /* factor n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* compute arithmetic derivative of n:
       derivative = sum_{p^e||n} e * n / p   */
    seq_t deriv = 0;
    seq_t seq_n = (seq_t)n;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t m = seq_n / (seq_t)primes[i];
        deriv += (seq_t)exps[i] * m;
    }

    /* factor the derivative to compute its rad (largest square‑free divisor) */
    uint64_t rad_primes[64];
    uint64_t rad_exps[64];
    size_t rad_cnt;

    /* derivative fits in uint64_t for all n up to 10000 (and the test set) */
    factor_u64((uint64_t)deriv, rad_primes, rad_exps, &rad_cnt);

    seq_t rad = 1;
    for (size_t i = 0; i < rad_cnt; ++i) {
        rad *= (seq_t)rad_primes[i];
    }

    /* result: derivative divided by its rad */
    return deriv / rad;
}
