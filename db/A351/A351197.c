/*
 * A351197  Sum of the 9th powers of the primes dividing n.
 * https://oeis.org/A351197
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{p|n, p prime} p^9
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 12, u64 n <= 138,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n) {
    uint64_t x = (uint64_t)n;
    seq_t sum = 0;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    factor_u64(x, primes, exps, &cnt);

    for (size_t i = 0; i < cnt; ++i) {
        sum += pow_uint(primes[i], (idx_t)9);
    }

    return sum;
}
