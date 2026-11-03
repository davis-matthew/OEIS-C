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
 * Validated: standard, pass, 103 terms through n = 9991 (2026-11-01)
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"

/* Recursive helper: count exponent vectors for primes[0..cnt-1]
   such that the sum of exponents ≤ rem and the product of primes^exponents
   (cur) is > limit. */
static seq_t _count_m(const uint64_t primes[], size_t cnt,
                      size_t idx, unsigned rem, seq_t cur, seq_t limit)
{
    if (idx == cnt) {
        return cur > limit ? 1 : 0;
    }
    seq_t total = 0;
    const uint64_t p = primes[idx];
    seq_t pow_val = 1;          /* p^0 */
    for (unsigned e = 0; e <= rem; ++e) {
        total += _count_m(primes, cnt, idx + 1, rem - e,
                          cur * pow_val, limit);
        pow_val = pow_val * (seq_t)p;  /* prepare p^(e+1) */
    }
    return total;
}

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
