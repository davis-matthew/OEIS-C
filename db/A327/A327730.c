/*
 * A327730  a(n) = A060594(2n).
 * https://oeis.org/A327730
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = 2^omega(n) if n mod 4 != 0, otherwise 2^(omega(n)+1)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_distinct_primes, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/count_distinct_primes.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    int omega = count_distinct_primes((uint64_t)n);
    idx_t exp = omega;
    if ((n & 3) == 0)               /* divisible by 4 */
        exp = (idx_t)omega + 1;
    return pow2(exp);
}
