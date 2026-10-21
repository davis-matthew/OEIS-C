/*
 * A281504  Solutions y to the negative Pell equation y^2 = 33*x^2 - 8 with
 *          x,y >= 0.
 * https://oeis.org/A281504
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 46*a(n-2) - a(n-4) (n>4) with a(1)=5, a(2)=17, a(3)=247,
 *            a(4)=787
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                      /* undefined for n<1, return 0 */
    if (n == 1) return 5;
    if (n == 2) return 17;
    if (n == 3) return 247;
    if (n == 4) return 787;

    /* Work with the last four values
       a_nm4 = a(n-4), a_nm3 = a(n-3), a_nm2 = a(n-2), a_nm1 = a(n-1) */
    seq_t a_nm4 = 5, a_nm3 = 17, a_nm2 = 247, a_nm1 = 787;
    seq_t val = 0;

    for (idx_t i = 5; i <= n; ++i) {
        /* a(i) = 46*a(i-2) - a(i-4) */
        val = ((seq_t)46) * a_nm2 - a_nm4;

        /* shift the window for the next iteration */
        a_nm4 = a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = val;
    }
    return a_nm1;
}
