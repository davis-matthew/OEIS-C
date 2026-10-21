/*
 * A253637  Second partial sums of ninth powers (A001017).
 * https://oeis.org/A253637
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + n^9 Initial values: a(1) = 1, a(2) =
 *            514
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 10, u64 n <= 85, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;      /* offset starts at 1, only for safety */

    if (n == 1)
        return (seq_t)1;      /* a(1) */

    if (n == 2)
        return (seq_t)514;    /* a(2) */

    /* compute iteratively using the recurrence */
    seq_t a_nm2 = (seq_t)1;      /* a(1)   */
    seq_t a_nm1 = (seq_t)514;    /* a(2)   */
    seq_t a_n   = 0;             /* placeholder for a(i) */

    for (idx_t i = 3; i <= n; ++i) {
        /* compute i^9 */
        seq_t i9 = (seq_t)i;
        for (idx_t k = 1; k < 9; ++k)
            i9 *= (seq_t)i;

        /* a(i) = 2*a(i-1) - a(i-2) + i^9 */
        a_n = a_nm1 * (seq_t)2 - a_nm2 + i9;

        /* shift for next iteration */
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }

    return a_n;   /* a(n) */
}
