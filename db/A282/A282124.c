/*
 * A282124  Decimal representation of the x-axis, from the origin to the right
 *          edge, of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 430", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A282124
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Equivalent recurrence a(0)=1, a(1)=3, a(2)=3, a(3)=15,
 *            a(n)=5*a(n-2)-4*a(n-4) for n>=4
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)3;
    if (n == 3) return (seq_t)15;

    /* Hold the last four values: arr[0]=a(n-4), arr[1]=a(n-3),
       arr[2]=a(n-2), arr[3]=a(n-1) for the current n in the loop. */
    seq_t arr[4] = {(seq_t)1, (seq_t)3, (seq_t)3, (seq_t)15};

    idx_t k;
    for (k = 4; k <= n; ++k)
    {
        /* Indices modulo 4 for the needed terms */
        size_t i2 = (size_t)((k - 2) & 3);   /* a(k-2) */
        size_t i4 = (size_t)((k - 4) & 3);   /* a(k-4) */

        seq_t val = ((seq_t)5) * arr[i2] - ((seq_t)4) * arr[i4];
        arr[(size_t)(k & 3)] = val;
    }
    return arr[(size_t)(n & 3)];
}
