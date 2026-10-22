/*
 * A389775  Absolute value of the inverse Möbius transform of mu(n)*phi(n^2).
 * https://oeis.org/A389775
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = product_{p|n} (p^2 - p - 1) over all distinct prime
 *            divisors p of n
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return 1;

    uint64_t primes[64];
    uint64_t exps[64];
    size_t  cnt = 0;
    factor_u64(n, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t p   = (seq_t)primes[i];
        seq_t val = p * p - p - 1;
        result *= val;
    }
    return result;
}
