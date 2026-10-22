/*
 * A380986  Product of prime indices of n (with multiplicity) minus product of
 *          distinct prime indices of n.
 * https://oeis.org/A380986
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A003963(n) - A156061(n)
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   compute_pi, factor_u64, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/compute_pi.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    if (n <= 1) return 0;

    /* factor_u64 will fill primes and their exponents.
       Allocate a small static buffer because the number
       of distinct prime factors of a 64‑bit integer is < 20. */
    uint64_t primes[32];
    uint64_t exps[32];
    size_t cnt = 0;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t prod_mult = 1;     /* product of prime indices with multiplicity */
    seq_t prod_dist = 1;     /* product of distinct prime indices */

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        idx_t e   = (idx_t)exps[i];

        /* Index of prime p is the number of primes ≤ p */
        idx_t idx = (idx_t)compute_pi(p);

        /* Multiply the index raised to its multiplicity */
        prod_mult *= pow_uint((uint64_t)idx, e);

        /* Multiply the distinct index once */
        prod_dist *= (seq_t)idx;
    }

    return prod_mult - prod_dist;
}
