/*
 * A277955  Decimal representation of the x-axis, from the origin to the right
 *          edge, of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 14", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A277955
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3 - 2*(-1)^n + 2^(1+n))/3 Initial values: a(0)=1,
 *            a(1)=3, a(2)=3 Recurrence for n≥3: a(n) = 2*a(n-1) + a(n-2) -
 *            2*a(n-3)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Handle the initial values directly */
    if (n == 0) return (seq_t)1;
    if (n == 1 || n == 2) return (seq_t)3;

    /* Use a sliding window of the last three terms to evaluate the recurrence */
    seq_t a0 = 1;       /* a(n-3)   */
    seq_t a1 = 3;       /* a(n-2)   */
    seq_t a2 = 3;       /* a(n-1)   */
    seq_t an = 0;       /* a(n)     */

    for (idx_t i = 3; i <= n; ++i)
    {
        /* an = 2*a2 + a1 - 2*a0 */
        an = (a2 << 1) + a1;     /* 2*a2 + a1  */
        an -= (a0 << 1);         /* subtract 2*a0 */

        /* slide the window forward */
        a0 = a1;
        a1 = a2;
        a2 = an;
    }

    return an;
}
