/*
 * A380955  Sum of prime indices of n (with multiplicity) minus sum of
 *          distinct prime indices of n.
 * https://oeis.org/A380955
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A056239(n) - A066328(n) (sum of prime indices – distinct
 *            sum)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   compute_pi, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/compute_pi.h"

/* Computes A380955: for n ≥ 1, return the sum of prime indices of n (with multiplicity)
   minus the sum of distinct prime indices of n.  */
seq_t a(idx_t n)
{
    if (n < 2) {          /* n == 1 */
        return 0;
    }

    /* Buffers for factor_u64 (at most 64 distinct prime factors for a 64‑bit integer). */
    static uint64_t primes[64];
    static uint64_t exps[64];
    size_t cnt = 0;

    factor_u64(n, primes, exps, &cnt);

    seq_t result = 0;
    for (size_t i = 0; i < cnt; ++i) {
        if (exps[i] <= 1) {
            continue;      /* (e-1) would be zero */
        }
        /* Prime index of the i-th prime factor.  */
        seq_t pi = compute_pi(primes[i]);     /* A056239(p) = PrimePi(p) */
        seq_t add = pi * (seq_t)(exps[i] - 1);
        result += add;
    }

    return result;
}
