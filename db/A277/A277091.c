/*
 * A277091  a(n) = ((1 + sqrt(15))^n - (1 - sqrt(15))^n)/sqrt(15).
 * https://oeis.org/A277091
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((1 + sqrt(15))^n - (1 - sqrt(15))^n)/sqrt(15).
 *            Recurrence: a(0)=0, a(1)=2, a(n)=2*a(n-1)+14*a(n-2) for n>=2
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    if (n == 1)
        return (seq_t)2;

    seq_t prev2 = (seq_t)0;   /* a(0) */
    seq_t prev1 = (seq_t)2;   /* a(1) */
    seq_t curr = 0;

    for (idx_t i = 2; i <= n; ++i) {
        /* a(i) = 2*a(i-1) + 14*a(i-2) */
        curr = (seq_t)2 * prev1 + (seq_t)14 * prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}
