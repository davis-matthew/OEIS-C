/*
 * A368555  a(n) = Sum_{k=0..n} k! * 3^(n-k).
 * https://oeis.org/A368555
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + n!, a(0) = 1
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = (seq_t)1;        /* a(0) */
    seq_t fact   = (seq_t)1;        /* 0! = 1 */

    for (idx_t i = 1; i <= n; ++i)
    {
        fact *= (seq_t)i;           /* i! = (i-1)! * i */
        result = result * (seq_t)3 + fact;  /* a(i) = 3*a(i-1) + i! */
    }

    return result;
}
