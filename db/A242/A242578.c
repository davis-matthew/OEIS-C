/*
 * A242578  Number of length n+3 0..2 arrays with no four elements in a row
 *          with pattern aabb (with a!=b) and new values 0..2 introduced in
 *          0..2 order.
 * https://oeis.org/A242578
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - 5*a(n-2) + 6*a(n-3) - 4*a(n-4) (n ≥ 5) Initial
 *            values: a(1)=13, a(2)=35, a(3)=97, a(4)=271
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 20, u64 n <= 41, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle indices below the offset */
    if (n < 1) return (seq_t)0;

    /* Return precomputed initial values */
    if (n == 1) return (seq_t)13;
    if (n == 2) return (seq_t)35;
    if (n == 3) return (seq_t)97;
    if (n == 4) return (seq_t)271;

    /* Compute iteratively using the recurrence */
    seq_t a1 = (seq_t)13; /* a(1) */
    seq_t a2 = (seq_t)35; /* a(2) */
    seq_t a3 = (seq_t)97; /* a(3) */
    seq_t a4 = (seq_t)271;/* a(4) */
    seq_t an = a4;       /* placeholder */

    for (idx_t i = 5; i <= n; ++i)
    {
        an = (seq_t)(4 * a4 - 5 * a3 + 6 * a2 - 4 * a1);
        /* Shift the window forward */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = an;
    }
    return an;
}
