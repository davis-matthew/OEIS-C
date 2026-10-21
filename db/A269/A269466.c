/*
 * A269466  Number of length-n 0..7 arrays with no repeated value equal to the
 *          previous repeated value.
 * https://oeis.org/A269466
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 13*a(n-1) - 28*a(n-2) - 98*a(n-3) for n>5; initial
 *            values: a(1)=8, a(2)=64, a(3)=504, a(4)=3976, a(5)=31304
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first five terms directly. */
    if (n == 1) return (seq_t)8;
    if (n == 2) return (seq_t)64;
    if (n == 3) return (seq_t)504;
    if (n == 4) return (seq_t)3976;
    if (n == 5) return (seq_t)31304;

    /* For n >= 6, use the linear recurrence. */
    seq_t a_nm3 = (seq_t)504;   /* a(3)  */
    seq_t a_nm2 = (seq_t)3976;  /* a(4)  */
    seq_t a_nm1 = (seq_t)31304; /* a(5)  */
    seq_t a_n;                  /* a(n)  */

    for (idx_t i = 6; i <= n; ++i)
    {
        a_n = (seq_t)13 * a_nm1 - (seq_t)28 * a_nm2 - (seq_t)98 * a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_n;
}
