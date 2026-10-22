/*
 * A318376  a(n) = F(n+1)^3 - 3*F(n-1)*F(n)^2, where F(n) = A000045(n), the
 *          n-th Fibonacci number.
 * https://oeis.org/A318376
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = F(n+1)^3 - 3*F(n-1)*F(n)^2 Recurrence: a(0)=1, a(1)=1,
 *            a(2)=5, a(3)=15, a(n)=3*a(n-1)+6*a(n-2)-3*a(n-3)-a(n-4) for n >
 *            3
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 31, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first four values explicitly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)5;
    if (n == 3) return (seq_t)15;

    /* Use the linear recurrence to compute up to n. */
    seq_t a0 = (seq_t)1;   /* a(n-3)  */
    seq_t a1 = (seq_t)1;   /* a(n-2)  */
    seq_t a2 = (seq_t)5;   /* a(n-1)  */
    seq_t a3 = (seq_t)15;  /* a(n)    */

    for (idx_t i = 4; i <= n; ++i)
    {
        seq_t an = 3 * a3 + 6 * a2 - 3 * a1 - a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }
    return a3;
}
