/*
 * A305290  Numbers k such that 4*k + 1 is a perfect cube, sorted by absolute
 *          values.
 * https://oeis.org/A305290
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   Let m = n-1 (so a(1)=0). Then if m is even: a(n) = m*(4*m*m +
 *            6*m + 3)/2 otherwise: a(n) = -(m+1)*(4*m*m + 2*m + 1)/2 This
 *            matches the OEIS terms: a(1)=0, a(2)=-7, a(3)=31,
 * Range:     i8 n <= 3, i16 n <= 21, i32 n <= 842, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is 1‑based index; convert to 0‑based m */
    idx_t m = n - 1;                 /* unsigned subtraction */
    seq_t mm = (seq_t)m;             /* cast to seq_t for arithmetic */

    if ((m & 1) == 0) /* m even */
    {
        seq_t term = (seq_t)4 * mm * mm + (seq_t)6 * mm + 3;
        return mm * term / 2;
    }
    else             /* m odd */
    {
        seq_t term = (seq_t)4 * mm * mm + (seq_t)2 * mm + 1;
        seq_t prod = (mm + 1) * term;
        return - (prod / 2);
    }
}
