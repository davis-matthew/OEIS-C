/*
 * A345668  Last prime minus distance to last prime.
 * https://oeis.org/A345668
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*P(n) – n, where P(n) is the largest prime < n
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

/* Return a(n) for n >= 3.  For n < 3 the value is undefined for the
   sequence and we simply return 0.  The function works for any
   integer type seq_t supported by the build. */
seq_t a(idx_t n)
{
    if (n < 3)
        return (seq_t)0;             /* value irrelevant below offset */

    /* Find the largest prime less than n */
    idx_t m = n - 1;
    while (m >= 2 && !is_prime(m))
        --m;                         /* m is now the largest prime < n */

    /* Compute 2*p – n in the target types, guarding against
       intermediate overflow by performing the calculation in seq_t. */
    seq_t res = (seq_t)2 * (seq_t)m - (seq_t)n;
    return res;
}
