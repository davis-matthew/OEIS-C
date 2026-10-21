/*
 * A250554  Number of length n+2 0..1 arrays with the sum of second
 *          differences multiplied by some arrangement of +-1 equal to zero.
 * https://oeis.org/A250554
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A250554:  Number of length n+2 0..1 arrays with the sum of
 * second differences multiplied by some arrangement of ±1 equal to zero.
 * Formula:  a(n) = 2^(n+1)             for even n,
 *            a(n) = 2^(n+1) - 2       for odd n.
 * The offset in OEIS is 1, so a(1) = 2.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)          /* offset is 1; behaviour for n==0 is irrelevant */
        return (seq_t)0;

    /* Compute 2^(n+1) */
    seq_t res = pow2(n + 1);

    /* For odd n subtract 2 */
    if (n & (idx_t)1)
        res = res - (seq_t)2;

    return res;
}
