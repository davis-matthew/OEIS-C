/*
 * A336530  Number of triples of divisors d_i < d_j < d_k of n such that
 *          gcd(d_i, d_j, d_k) > 1.
 * https://oeis.org/A336530
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the number of triples of divisors d_i < d_j < d_k of n *
 *            with gcd(d_i, d_j, d_k) > 1. * * Inclusion–exclusion over the
 *            prime factors p_i of n: * Let e_i be the exponent of p_i in n. *
 *            For a non‑empty subset S of primes, let * f(S) = ∏_{i∈S} e_i ·
 *            ∏_{i∉S} (e_i + 1) * (f(S) is the number of...
 * Range:     u8 n <= 119, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
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
    if (n < 1)
        return 0;

    uint64_t nn = (uint64_t)n;
    /* arrays for prime factors and exponents, up to 32 distinct primes */
    uint64_t primes[32];
    uint64_t exps[32];
    size_t  cnt;

    factor_u64(nn, primes, exps, &cnt);

    if (cnt == 0 || nn == 1)
        return 0;

    seq_t answer = 0;

    /* iterate over all non‑empty subsets of the prime set */
    for (size_t mask = 1; mask < ((size_t)1 << cnt); ++mask) {
        seq_t f = 1;
        size_t bits = 0;      /* number of primes in the subset */

        for (size_t i = 0; i < cnt; ++i) {
            if (mask & ((size_t)1 << i)) {
                f *= (seq_t)exps[i];
                ++bits;
            } else {
                f *= (seq_t)(exps[i] + 1);
            }
        }

        if (f >= 3) {
            seq_t comb = f * (f - 1) * (f - 2) / 6;
            if (bits & 1)
                answer += comb;   /* odd subset size → + */
            else
                answer -= comb;   /* even subset size → – */
        }
    }

    return answer;
}
