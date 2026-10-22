/*
 * A376847  Number of m > n such that rad(m) | n and Omega(m) <= Omega(n),
 *          where rad = A007947 and Omega = A001222.
 * https://oeis.org/A376847
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # { m > n : rad(m) | n and Ω(m) ≤ Ω(n) }
 * Range:     u8 n <= 33, u16 n <= 5821, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _count_m, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/_count_m.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    uint64_t N = (uint64_t)n;
    size_t cnt = 0;
    uint64_t primes[64];
    uint64_t exps[64];
    factor_u64(N, primes, exps, &cnt);

    unsigned omega_n = 0;
    for (size_t i = 0; i < cnt; ++i)
        omega_n += (unsigned)exps[i];

    seq_t limit = (seq_t)N;
    return _count_m(primes, cnt, 0, omega_n, (seq_t)1, limit);
}
