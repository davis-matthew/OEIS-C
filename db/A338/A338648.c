/*
 * A338648  Number of divisors of n which are greater than 4.
 * https://oeis.org/A338648
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = d(n) - (1 + [2|n] + [3|n] + [4|n])
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where d(n) is the number of positive divisors of n. */

#include <stdint.h>
#include "../utils/divisor_count_u64.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* safeguard, unused for OEIS offset 1 */

    seq_t d = (seq_t)divisor_count_u64((uint64_t)n);
    seq_t small = 1;                     /* divisor 1 always */

    if ((n & 1) == 0) small++;           /* 2 divides n */
    if (n % 3 == 0) small++;             /* 3 divides n */
    if ((n & 0x3) == 0) small++;         /* 4 divides n */

    return d - small;
}
