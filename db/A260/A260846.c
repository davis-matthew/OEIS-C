/*
 * A260846  a(n) = (-3 - 28*3^n + 73*15^n)/21.
 * https://oeis.org/A260846
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (-3 - 28*3^n + 73*15^n)/21
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent recurrence: a(0)=2; a(n)=15*a(n-1)+2+16*3^(n-1) for n>0 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)2;

    seq_t a_val = (seq_t)2;     /* a(0) */
    seq_t pow3  = (seq_t)1;     /* 3^(i-1), starts with 3^0 */

    for (idx_t i = 1; i <= n; ++i)
    {
        a_val = a_val * (seq_t)15 + (seq_t)2 + (seq_t)16 * pow3;
        pow3  = pow3 * (seq_t)3;   /* advance to the next 3 power */
    }

    return a_val;
}
