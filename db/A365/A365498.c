/*
 * A365498  Dirichlet g.f.: zeta(s) * Product_{p prime} (1 + 1/p^s -
 *          1/p^(3*s)).
 * https://oeis.org/A365498
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = ∏_{p|n} (2 if exponent of p in n ≤ 2, otherwise 1) The
 *            function returns the number of unitary divisors of n that are
 *            cubefree
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/factor_u64.h"

/* Computes A365498: number of unitary divisors of n that are cubefree. */
seq_t a(idx_t n)
{
    /* Handle the trivial case n=1. */
    if (n == 0) return 0;          /* offset starts at 1, but guard just in case */

    /* Factor n: arrays large enough for any 64‑bit input. */
    size_t cnt = 0;
    uint64_t primes[64];
    uint64_t exps[64];
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t res = 1;
    for (size_t i = 0; i < cnt; ++i) {
        if (exps[i] <= 2) {
            res *= 2;
        }
    }
    return res;
}
