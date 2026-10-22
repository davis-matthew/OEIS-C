/*
 * A373957  Greatest number of runs in a permutation of the prime factors of
 *          n.
 * https://oeis.org/A373957
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = N if max_exp ≤ sum_other + 1 else 2*sum_other + 1 where N
 *            is the total number of prime factors of n (counted with
 *            multiplicity), max_exp is the largest exponent among the prime
 *            factorization of n, and sum_other = N - max_exp. This computes
 *            the maximal number of runs in any permutation of t...
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)0;

    /* A small fixed-size buffer is sufficient: the maximum
       number of distinct prime factors for a 64‑bit number is < 64. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t total   = 0;   /* N */
    seq_t max_exp = 0;   /* largest exponent */
    for (size_t i = 0; i < cnt; ++i) {
        seq_t e = (seq_t)exps[i];
        total   += e;
        if (e > max_exp)
            max_exp = e;
    }

    seq_t sum_other = total - max_exp;
    if (max_exp <= sum_other + (seq_t)1)
        return total;            /* all elements can be isolated */
    else
        return 2 * sum_other + (seq_t)1;  /* limited by the largest multiplicity */
}
