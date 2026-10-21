/*
 * A249458  The numerators of curvatures of touching circles inscribed in a
 *          special way in the smaller segment of unit circle divided by a
 *          chord of length sqrt(84)/5.
 * https://oeis.org/A249458
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 33*a(n-1) - 231*a(n-2) + 343*a(n-3)  for n>=3,
   with a(0)=10, a(1)=100, a(2)=1690. */

seq_t a(idx_t n)
{
    /* Handle initial terms */
    if (n == 0) return (seq_t)10;
    if (n == 1) return (seq_t)100;
    if (n == 2) return (seq_t)1690;

    /* Use a sliding window of the last three values */
    seq_t a_nm3 = (seq_t)10;     /* a(n-3) */
    seq_t a_nm2 = (seq_t)100;    /* a(n-2) */
    seq_t a_nm1 = (seq_t)1690;   /* a(n-1) */
    seq_t cur;

    for (idx_t i = 3; i <= n; ++i) {
        /* 33*a_nm1 - 231*a_nm2 + 343*a_nm3  */
        cur = (seq_t)33 * a_nm1 - (seq_t)231 * a_nm2 + (seq_t)343 * a_nm3;

        /* Shift the window for next iteration */
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = cur;
    }

    return a_nm1;
}
