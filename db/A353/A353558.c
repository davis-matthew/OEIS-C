/*
 * A353558  a(n) = 1 if n is an odd number with an odd number of prime factors
 *          (counted with multiplicity), otherwise 0.
 * https://oeis.org/A353558
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n is odd with an odd number of prime factors *
 *            (counted with multiplicity); otherwise 0
 * Range:     u8 n <= 9340, u16 n <= 9340, u32 n <= 9340, u64 n <= 9340,
 *            u128 n <= 9340, big any n
 * Helpers:   bigomega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/bigomega.h"

seq_t a(idx_t n)
{
    /* Return 0 for even n. */
    if ((n & (idx_t)1) == 0)
        return 0;

    /* Count prime factors with multiplicity. */
    seq_t cnt = bigomega(n);

    /* Return 1 if cnt is odd, else 0. */
    return cnt & 1;
}
