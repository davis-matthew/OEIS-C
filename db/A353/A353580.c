/*
 * A353580  a(n) = 2*a(n-1) + a(n-2) - 1, with a(0) = 0 and a(1) = 2.
 * https://oeis.org/A353580
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + a(n-2) - 1, a(0) = 0, a(1) = 2
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 26, u64 n <= 51, u128 n <= 101,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 2;

    seq_t prev2 = 0;   /* a(n-2)   */
    seq_t prev1 = 2;   /* a(n-1)   */
    seq_t curr  = 0;   /* a(n)     */

    for (idx_t i = 2; i <= n; ++i) {
        curr = 2 * prev1 + prev2 - 1;
        prev2 = prev1;
        prev1 = curr;
    }
    return curr;
}
