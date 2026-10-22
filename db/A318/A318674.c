/*
 * A318674  Sum of squarefree divisors of n that have an even number of prime
 *          factors.
 * https://oeis.org/A318674
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 209, u16 n <= 9710, u32 n <= 9710, u64 n <= 9710,
 *            u128 n <= 9710, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = Sum_{d|n} [squarefree(d) && omega(d) even] * d
 * (where omega(d) is the number of distinct prime factors of d)
 *
 * Computes a(n) for n >= 1 with a simple factorization followed by
 * enumeration of all square‑free divisors whose number of prime factors
 * is even.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, but harmless */

    /* Factorize n.  The helper stores distinct primes in pr[].  */
    uint64_t pr[64];
    uint64_t ex[64];               /* exponents are unused */
    size_t cnt = 0;
    factor_u64(n, pr, ex, &cnt);

    seq_t sum = 0;

    /* Enumerate all subsets of distinct prime factors.  */
    unsigned total_masks = 1U << (unsigned)cnt;
    for (unsigned mask = 0; mask < total_masks; ++mask) {
        seq_t prod = 1;
        unsigned parity = 0;          /* parity of number of primes in subset */
        for (unsigned i = 0; i < (unsigned)cnt; ++i) {
            if (mask & (1U << i)) {
                prod *= (seq_t)pr[i];
                parity ^= 1;
            }
        }
        if (parity == 0)              /* even number of prime factors */
            sum += prod;
    }
    return sum;
}
