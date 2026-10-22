/*
 * A360908  Multiplicative with a(p^e) = 2*e - 1.
 * https://oeis.org/A360908
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = ∏_{p^e‖n} (2*e – 1) (multiplicative, a(p^e) = 2e−1)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    /* arrays for primes and exponents; 64 is enough for 64-bit n */
    uint64_t primes[64];
    uint64_t exps [64];
    size_t   cnt   = 0;

    factor_u64(n, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i)
    {
        seq_t e   = (seq_t)exps[i];
        seq_t term = e * 2 - 1;   /* 2*e - 1 */
        result = result * term;
    }
    return result;
}
