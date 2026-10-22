/*
 * A321715  a(n) = 3*a(n-1) + 4*a(n-2) + a(n-3), a(0) = 1, a(1) = -1, a(2) =
 *          -1 .
 * https://oeis.org/A321715
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 3*a(n-1) + 4*a(n-2) + a(n-3), a(0)=1, a(1)=-1, a(2)=-1
 * Range:     i8 n <= 5, i16 n <= 9, i32 n <= 17, i64 n <= 32, i128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)-1;
    if (n == 2) return (seq_t)-1;

    seq_t a0 = (seq_t)1;   /* a(0) */
    seq_t a1 = (seq_t)-1;  /* a(1) */
    seq_t a2 = (seq_t)-1;  /* a(2) */

    for (idx_t i = 3; i <= n; ++i) {
        seq_t an = (seq_t)3 * a2 + (seq_t)4 * a1 + a0;
        a0 = a1;
        a1 = a2;
        a2 = an;
    }
    return a2;
}
