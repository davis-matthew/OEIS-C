/*
 * A332476  The number of unitary divisors of n in Gaussian integers.
 * https://oeis.org/A332476
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = ∏_{p|n} (4 if p ≡ 1 (mod 4) else 2)
 * Range:     u8 n <= 6629, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)1;
    uint64_t num = (uint64_t)n;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(num, primes, exps, &cnt);
    seq_t res = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        if ((primes[i] & 3) == 1)
            res *= (seq_t)4;
        else
            res *= (seq_t)2;
    }
    return res;
}
