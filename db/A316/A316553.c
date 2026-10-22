/*
 * A316553  Number of elements of order 2 in the group SL(2, Z(n)).
 * https://oeis.org/A316553
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(omega(n) + min(3, valuation(n, 2))) - 1
 * Range:     u8 n <= 1023, u16 n <= 1023, u32 n <= 1023, u64 n <= 1023,
 *            u128 n <= 1023, big any n
 * Helpers:   count_distinct_primes, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/count_distinct_primes.h"
#include "../utils/pow2.h"
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* for completeness – n>=1 in use */

    /* valuation of 2 in n */
    idx_t v2 = 0;
    idx_t tmp = n;
    while ((tmp & 1ULL) == 0) {
        v2++;
        tmp >>= 1;
    }

    /* number of distinct prime factors */
    int omega_int = count_distinct_primes((uint64_t)n);
    idx_t omega = (idx_t)omega_int;

    /* exponent for 2^exp */
    idx_t exp = omega + (v2 < 3 ? v2 : 3ULL);

    /* 2^exp minus 1 */
    seq_t val = pow2(exp);
    val -= (seq_t)1;
    return val;
}
