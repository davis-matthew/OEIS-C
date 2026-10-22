/*
 * A338293  Number of matchings in the complete binary tree of n rows.
 * https://oeis.org/A338293
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(1) = 1, a(n) = a(n-1)^2 + 2*a(n-1)*a(n-2)^2 for n ≥
 *            2
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for idx_t */

seq_t a(idx_t n)
{
    /* Handle the first two terms directly */
    if (n == 0 || n == 1)
        return (seq_t)1;

    seq_t a_nm2 = (seq_t)1;  /* a(0) */
    seq_t a_nm1 = (seq_t)1;  /* a(1) */
    seq_t a_n   = 0;         /* placeholder for a(k) */

    for (idx_t i = 2; i <= n; ++i)
    {
        /* a(n-1)^2 */
        seq_t term1 = a_nm1 * a_nm1;

        /* a(n-2)^2 */
        seq_t a_nm2_sq = a_nm2 * a_nm2;

        /* 2 * a(n-1) * a(n-2)^2 */
        seq_t term2 = a_nm1 * a_nm2_sq;
        term2 *= 2;

        a_n = term1 + term2;

        /* rotate for the next iteration */
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }

    return a_nm1;
}
