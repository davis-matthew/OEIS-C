/*
 * A327527  Number of uniform divisors of n.
 * https://oeis.org/A327527
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Number of uniform divisors of n * a(n) = 1 + Σ_{k≥1}
 *            (2^{M_k} – 1) where M_k = #primes of n with exponent ≥ k
 * Range:     u8 n <= 9745, u16 n <= 9745, u32 n <= 9745, u64 n <= 9745,
 *            u128 n <= 9745, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    uint64_t nn = (uint64_t)n;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    /* factor n into primes and their exponents */
    factor_u64(nn, primes, exps, &cnt);

    if (cnt == 0)   /* n == 1 */
        return 1;

    unsigned int counts[65] = {0};
    unsigned int max_e = 0;

    for (size_t i = 0; i < cnt; ++i) {
        unsigned int e = (unsigned int)exps[i];
        counts[e]++;
        if (e > max_e)
            max_e = e;
    }

    seq_t res = 1;                  /* divisor 1 */
    unsigned int cumulative = 0;

    for (int k = max_e; k >= 1; --k) {
        cumulative += counts[k];
        /* all non‑empty subsets of the primes counted in cumulative */
        res += ((seq_t)1 << cumulative) - 1;
    }

    return res;
}
