/*
 * A369257  a(n) = number of odd divisors of n that have an even number of
 *          prime factors with multiplicity.
 * https://oeis.org/A369257
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of odd divisors of n with even total number of
 *            prime factors (multiplicity)
 * Range:     u8 n <= 9582, u16 n <= 9582, u32 n <= 9582, u64 n <= 9582,
 *            u128 n <= 9582, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* If n == 1, the only odd divisor is 1 and ω(1)=0 (even), hence a(1)=1. */
    if (n == 1) return (seq_t)1;

    /* Factor n using the provided utility. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t even = 1;   /* count of combinations with even total exponent sum so far */
    seq_t odd  = 0;   /* count of combinations with odd total exponent sum so far */

    for (size_t i = 0; i < cnt; ++i) {
        if (primes[i] == 2) continue;          /* ignore factor 2 – we only need odd divisors */

        uint64_t e = exps[i];

        /* Number of ways to choose an exponent with even parity from this prime factor. */
        seq_t even_k = (seq_t)(e / 2 + 1);          /* k = 0,2,4,... ≤ e   */
        /* Number of ways to choose an exponent with odd parity. */
        seq_t odd_k  = (seq_t)((e + 1) / 2);       /* k = 1,3,5,... ≤ e   */

        seq_t new_even = even * even_k + odd * odd_k;
        seq_t new_odd  = even * odd_k  + odd * even_k;

        even = new_even;
        odd  = new_odd;
    }

    /* 'even' now counts all odd divisors of n with an even total number of prime factors. */
    return even;
}
