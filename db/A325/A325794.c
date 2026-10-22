/*
 * A325794  Number of divisors of n minus the sum of prime indices of n.
 * https://oeis.org/A325794
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, look, hear
 * Formula:   a(n) = A000005(n) - A056239(n)
 * Range:     i8 n <= 738, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   factor_u64, prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/prime_pi.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset is 1; n=0 is unused */

    uint64_t pr[64];
    uint64_t ex[64];
    size_t cnt = 0;

    /* factor_u64 fills pr[], ex[] with distinct primes and their exponents,
       and sets cnt to the number of distinct prime factors. */
    factor_u64((uint64_t)n, pr, ex, &cnt);

    seq_t d = 1;    /* divisor count */
    seq_t sum = 0;  /* sum of prime indices with multiplicity */

    for (size_t i = 0; i < cnt; ++i) {
        d   *= (seq_t)(ex[i] + 1);
        sum += (seq_t)ex[i] * (seq_t)prime_pi((idx_t)pr[i]);   /* prime_pi(p)=index of p */
    }

    return d - sum;
}
