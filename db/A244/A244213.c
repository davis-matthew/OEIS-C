/*
 * A244213  Inverse binomial transform of -2 followed by A000032(n+1).
 * https://oeis.org/A244213
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -2*a(n-1) + a(n-3) with initial values: a(0) = -2, a(1) =
 *            3, a(2) = -1 This recurrence is equivalent to the definitions
 *            given for sequence A244213
 * Range:     i8 n <= 10, i16 n <= 21, i32 n <= 44, i64 n <= 90, i128 n <= 182,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first three terms directly. */
    if (n == 0) return (seq_t)-2;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)-1;

    /* a_nm3 = a(n-3), a_nm2 = a(n-2), a_nm1 = a(n-1)  */
    seq_t a_nm3 = (seq_t)-2; /* a(0) */
    seq_t a_nm2 = (seq_t)3;  /* a(1) */
    seq_t a_nm1 = (seq_t)-1; /* a(2) */

    /* Iteratively compute up to a(n). */
    for (idx_t i = 3; i <= n; ++i) {
        seq_t next = -( (seq_t)2 * a_nm1 ) + a_nm3; /* a(i) = -2*a(i-1) + a(i-3) */
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = next;
    }

    return a_nm1;
}
