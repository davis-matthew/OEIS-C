/*
 * A387539  Sum of the odd parts in the partitions of n into 2 parts.
 * https://oeis.org/A387539
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1..⌊n/2⌋} [k*(k mod 2) + (n-k)*((n-k) mod 2)]
 * Range:     u8 n <= 31, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t half = n / 2;          /* floor(n/2) */
    for (idx_t k = 1; k <= half; ++k) {
        if (k & 1ULL)            /* k is odd? */
            sum += (seq_t)k;
        idx_t nk = n - k;
        if (nk & 1ULL)           /* (n-k) is odd? */
            sum += (seq_t)nk;
    }
    return sum;
}
