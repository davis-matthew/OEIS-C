/*
 * A272564  Number of active (ON, black) cells in n-th stage of growth of
 *          two-dimensional cellular automaton defined by "Rule 501", based on
 *          the 5-celled von Neumann neighborhood.
 * https://oeis.org/A272564
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=8, a(2)=5, a(3)=44, a(4)=13, a(5)=112 For n>5: if n
 *            even: a(n) = 6*n-11 if n odd : a(n) = 4*n^2 + 2*n - 2
 * Range:     u8 n <= 8, u16 n <= 128, u32 n <= 128, u64 n <= 128,
 *            u128 n <= 128, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Initial values for n = 0..5 */
    static const seq_t init[6] = {1, 8, 5, 44, 13, 112};

    if (n < 6)
        return init[n];

    /* use int64_t for intermediate calculations to avoid overflow of signed constants */
    int64_t nn = (int64_t)n;

    if ((n & 1ULL) == 0)          /* n even */
    {
        return (seq_t)(6 * nn - 11);
    }
    else                          /* n odd */
    {
        return (seq_t)(4 * nn * nn + 2 * nn - 2);
    }
}
