/*
 * A383734  Numbers k such that 2+k and 2*k are squares.
 * https://oeis.org/A383734
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 34*a(n-1) - a(n-2) + 32;  a(1)=2, a(2)=98.  Offset 1. */

seq_t a(idx_t n)
{
    if (n == (idx_t)1) return (seq_t)2;
    if (n == (idx_t)2) return (seq_t)98;

    seq_t a_nm2 = (seq_t)2;           /* a(n-2) */
    seq_t a_nm1 = (seq_t)98;          /* a(n-1) */
    seq_t a_n   = 0;

    for (idx_t i = (idx_t)3; i <= n; ++i) {
        a_n   = (seq_t)34 * a_nm1 - a_nm2 + (seq_t)32;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_n;
}
