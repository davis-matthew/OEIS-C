/*
 * A254132  a(0)=1 and a(1)=2, then each term is x + y + x*y where x and y are
 *          the 2 last terms.
 * https://oeis.org/A254132
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=2, a(n)=a(n-1)+a(n-2)+a(n-1)*a(n-2) for n>=2
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 7, u64 n <= 8, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the two predefined terms */
    if (n == (idx_t)0) return (seq_t)1;
    if (n == (idx_t)1) return (seq_t)2;

    /* Iterative computation using the recurrence */
    seq_t prev2 = (seq_t)1; /* a(0) */
    seq_t prev1 = (seq_t)2; /* a(1) */
    for (idx_t i = (idx_t)2; i <= n; ++i) {
        seq_t cur = prev1 + prev2 + (prev1 * prev2);
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
