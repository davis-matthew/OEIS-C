/*
 * A373979  a(n) = 1 if n is a nonprime of the form 4*k + 3, otherwise 0.
 * https://oeis.org/A373979
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n is a nonprime of the form 4*k + 3, otherwise 0
 * Range:     u8 n <= 9631, u16 n <= 9631, u32 n <= 9631, u64 n <= 9631,
 *            u128 n <= 9631, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* Check for the 4k+3 condition and composite status */
    if ((n & 3ULL) == 3 && !is_prime(n))
        return (seq_t)1;
    return (seq_t)0;
}
