/*
 * A277782  Number of n X 2 0..2 arrays with every element equal to some
 *          element at offset (-1,-1) (-1,0) (-1,1) (0,-1) (0,1) (1,-1) or
 *          (1,0) both plus 1 mod 3 and minus 1 mod 3, with new values
 *          introduced in order 0..2.
 * https://oeis.org/A277782
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) + 8*a(n-2) - 6*a(n-3) Valid for n >= 4, with
 *            initial values a(1)=0, a(2)=3, a(3)=12
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 26, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == (idx_t)1) return (seq_t)0;
    if (n == (idx_t)2) return (seq_t)3;
    if (n == (idx_t)3) return (seq_t)12;

    seq_t a1 = (seq_t)0;     /* a(n-3) */
    seq_t a2 = (seq_t)3;     /* a(n-2) */
    seq_t a3 = (seq_t)12;    /* a(n-1) */
    seq_t ai;

    for (idx_t i = (idx_t)4; i <= n; ++i)
    {
        ai = (seq_t)5 * a3 + (seq_t)8 * a2 - (seq_t)6 * a1;
        a1 = a2;
        a2 = a3;
        a3 = ai;
    }
    return a3;
}
