/*
 * A392137  a(n) = 5*a(n-1) + 2*5^n - 1, a(0) = 0.
 * https://oeis.org/A392137
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) + 2*5^n - 1, a(0)=0
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 25, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t a_prev = 0;   /* a(0) */
    seq_t pow5   = 1;   /* 5^0 */

    for (idx_t i = 1; i <= n; ++i) {
        pow5 = pow5 * (seq_t)5;                 /* 5^i */
        a_prev = 5 * a_prev + 2 * pow5 - (seq_t)1;
    }
    return a_prev;
}
