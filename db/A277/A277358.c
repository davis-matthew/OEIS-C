/*
 * A277358  Number of self-avoiding planar walks starting at (0,0), ending at
 *          (n,0), remaining in the first quadrant and using steps (0,1),
 *          (1,0), (1,1), (-1,1), and (1,-1).
 * https://oeis.org/A277358
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=2, a(n)=2*n*a(n-1)+(n-1)*a(n-2) for n>1 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Handle first two terms directly. */
    if (n == (idx_t)0)
        return (seq_t)1;
    if (n == (idx_t)1)
        return (seq_t)2;

    /* Two variables are enough to generate the sequence. */
    seq_t a_nm2 = (seq_t)1;   /* a(0) */
    seq_t a_nm1 = (seq_t)2;   /* a(1) */

    for (idx_t i = (idx_t)2; i <= n; ++i) {
        /* 2 * i * a_nm1 */
        seq_t term1 = a_nm1 * (seq_t)i;
        term1 += term1;

        /* (i-1) * a_nm2 */
        seq_t term2 = a_nm2 * (seq_t)(i - (idx_t)1);

        /* a(i) = term1 + term2 */
        seq_t cur = term1 + term2;

        /* Shift for next iteration. */
        a_nm2 = a_nm1;
        a_nm1 = cur;
    }

    return a_nm1;
}
