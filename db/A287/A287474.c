/*
 * A287474  Number of dominating sets in the n-web graph.
 * https://oeis.org/A287474
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) + 3*a(n-2) + 9*a(n-3), a(0)=3, a(1)=5, a(2)=31
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the three initial values directly. */
    if (n == 0) return (seq_t)3;
    if (n == 1) return (seq_t)5;
    if (n == 2) return (seq_t)31;

    /* Recurrence variables: previous three terms. */
    seq_t prev3 = (seq_t)3;  // a(0)
    seq_t prev2 = (seq_t)5;  // a(1)
    seq_t prev1 = (seq_t)31; // a(2)
    seq_t cur = 0;

    /* Compute values up to n in O(n) time. */
    for (idx_t i = 3; i <= n; ++i)
    {
        cur = 5 * prev1 + 3 * prev2 + 9 * prev3;
        prev3 = prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
