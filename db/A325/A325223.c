/*
 * A325223  Sum of the prime indices of n minus the greater of the number of
 *          prime factors of n counted with multiplicity and the largest prime
 *          index of n.
 * https://oeis.org/A325223
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A056239(n) - max(A001222(n), A061395(n))
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   compute_pi, factor_u64, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (Sum of prime indices of n) - max(number of prime factors with multiplicity,
   largest prime index of n) */

#include <stddef.h>
#include <stdint.h>

#include "../utils/is_prime.h"
#include "../utils/factor_u64.h"
#include "../utils/compute_pi.h"

/* Return the nth term of the sequence A325223. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* offset 1: a(1) is first term */
    if (n == 1)
        return 0;               /* a(1) = 0 */

    /* Prime numbers produce a(n)=0 directly. */
    if (is_prime(n))
        return 0;

    /* Factorize n. 64-bit n has at most 64 prime factors (2^64-1). */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t  cnt = 0;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t sum_indices = 0;
    seq_t total_mult  = 0;
    seq_t max_index   = 0;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t   p = primes[i];
        uint64_t   e = exps[i];
        seq_t      idx = (seq_t)compute_pi(p);   /* prime index of p */

        sum_indices += idx * (seq_t)e;
        total_mult += (seq_t)e;

        if (idx > max_index)
            max_index = idx;
    }

    seq_t max_val = (total_mult > max_index) ? total_mult : max_index;
    return sum_indices - max_val;
}
