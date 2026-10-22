/*
 * A322504  a(n) = -4*a(n-1) - 3*a(n-2) + a(n-3), a(0) = 1, a(1) = -2, a(2) =
 *          4.
 * https://oeis.org/A322504
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -4*a(n-1) - 3*a(n-2) + a(n-3), a(0) = 1, a(1) = -2, a(2)
 *            = 4
 * Range:     i8 n <= 5, i16 n <= 11, i32 n <= 22, i64 n <= 43, i128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first three terms directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)-2;
    if (n == 2) return (seq_t)4;

    /* Keep the last three computed values. */
    seq_t a_nm3 = (seq_t)1;   /* a(n-3)  */
    seq_t a_nm2 = (seq_t)-2;  /* a(n-2)  */
    seq_t a_nm1 = (seq_t)4;   /* a(n-1)  */
    seq_t a_curr;

    /* Compute iteratively up to the requested index. */
    for (idx_t i = 3; i <= n; ++i) {
        seq_t term1 = (seq_t)4 * a_nm1;      /* 4 * a(n-1) */
        seq_t term2 = (seq_t)3 * a_nm2;      /* 3 * a(n-2) */
        a_curr = -term1 - term2 + a_nm3;     /* -(4*a(n-1)) - (3*a(n-2)) + a(n-3) */
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_curr;
    }
    return a_curr;
}
