/*
 * A318876  Sum of divisors d of n for which 2*phi(d) > d.
 * https://oeis.org/A318876
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} [2*phi(d) > d]*d
 * Range:     u8 n <= 170, u16 n <= 9936, u32 n <= 9936, u64 n <= 9936,
 *            u128 n <= 9936, big any n
 * Helpers:   calc_phi, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/calc_phi.h"

static void enumerate_divisors(const uint64_t *primes, const uint64_t *exps,
                               size_t cnt, size_t idx, seq_t current, seq_t *sum)
{
    if (idx == cnt) {
        seq_t phi = calc_phi(current);
        if ((seq_t)2 * phi > current)
            *sum += current;
        return;
    }
    seq_t val = current;
    for (uint64_t e = 0; e <= exps[idx]; ++e) {
        enumerate_divisors(primes, exps, cnt, idx + 1, val, sum);
        val *= primes[idx];
    }
}

seq_t a(idx_t n)
{
    /* n is guaranteed to be >= 1 due to the OEIS offset */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t sum = (seq_t)0;
    enumerate_divisors(primes, exps, cnt, 0, (seq_t)1, &sum);
    return sum;
}
