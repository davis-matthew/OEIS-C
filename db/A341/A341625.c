/*
 * A341625  a(n) = 1 if the arithmetic derivative of n is less than n,
 *          otherwise 0.
 * https://oeis.org/A341625
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if A003415(n) < n, otherwise 0
 * Range:     u8 n <= 7596, u16 n <= 7596, u32 n <= 7596, u64 n <= 7596,
 *            u128 n <= 7596, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* a(0) = 0 */
    if (n == 1) return 1;            /* a(1) = 1 */

    uint64_t num = (uint64_t)n;
    uint64_t primes[16];
    uint64_t exps[16];
    size_t cnt = 0;

    /* Factorize n into primes and exponents */
    factor_u64(num, primes, exps, &cnt);

    double sum = 0.0;
    for (size_t i = 0; i < cnt; ++i) {
        sum += (double)exps[i] / (double)primes[i];
    }

    return (seq_t)(sum < 1.0);
}
