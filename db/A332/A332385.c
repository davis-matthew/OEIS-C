/*
 * A332385  Sum of squares of indices of distinct prime factors of n.
 * https://oeis.org/A332385
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{p|n, p prime} π(p)², where π(p) is the prime-counting
 *            function
 * Range:     u8 n <= 52, u16 n <= 1618, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   compute_pi, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the utility functions factor_u64 and compute_pi. */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/compute_pi.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;

    /* factor_u64 can handle at most 64 distinct prime factors of a 64‑bit integer. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t count = 0;

    factor_u64((uint64_t)n, primes, exps, &count);

    seq_t sum = 0;
    for (size_t i = 0; i < count; ++i) {
        seq_t pi_val = compute_pi((idx_t)primes[i]);   /* π(p) = index of prime p */
        sum += pi_val * pi_val;
    }

    return sum;
}
