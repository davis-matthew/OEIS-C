/*
 * A295619  a(n) = a(n-1) + 3*a(n-2) - 2*a(n-3) - 2*a(n-4), where a(0) = 1,
 *          a(1) = 2, a(2) = 3, a(3) = 4.
 * https://oeis.org/A295619
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 3*a(n-2) - 2*a(n-3) - 2*a(n-4), a(0) = 1, a(1) =
 *            2, a(2) = 3, a(3) = 4
 * Range:     u8 n <= 12, u16 n <= 24, u32 n <= 47, u64 n <= 93, u128 n <= 186,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Handle the initial four values directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)3;
    if (n == 3) return (seq_t)4;

    /* Iteratively compute the sequence up to n.
       We keep only the last four values to satisfy the recurrence.   */
    seq_t a0 = (seq_t)1;  /* a(n-4)   */
    seq_t a1 = (seq_t)2;  /* a(n-3)   */
    seq_t a2 = (seq_t)3;  /* a(n-2)   */
    seq_t a3 = (seq_t)4;  /* a(n-1)   */

    for (idx_t i = 4; i <= n; ++i) {
        seq_t an = a3 + 3 * a2 - 2 * a1 - 2 * a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }

    return a3;
}
