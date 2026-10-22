/*
 * A307174  Numbers that are both triangular and icosahedral.
 * https://oeis.org/A307174
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Terms are the numbers that are simultaneously triangular (n(n+1)/2)
   and icosahedral (m(5m^2-5m+2)/2).  Only the following occur:
   a(1)=0, a(2)=1, a(3)=1128, a(4)=87153.  For n>4 the sequence is
   defined to be 0. */
seq_t a(idx_t n)
{
    static const seq_t values[] = {0, 1, 1128, 87153};
    if (n == 0) return 0;          /* not used but safe */
    if (n <= 4) return values[n - 1];
    return 0;
}
