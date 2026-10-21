/*
 * A292400  p-INVERT of (1,2,2,2,2,2,2,...) (A040000), where p(S) = (1 - S)^2.
 * https://oeis.org/A292400
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 2*a(n-2) - 4*a(n-3) - a(n-4) (n >= 5); initial
 *            values: a(0)=2, a(1)=7, a(2)=20, a(3)=57, a(4)=158
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 22, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)7;
    if (n == 2) return (seq_t)20;
    if (n == 3) return (seq_t)57;
    if (n == 4) return (seq_t)158;

    seq_t a0 = (seq_t)7;   /* a(1)   */
    seq_t a1 = (seq_t)20;  /* a(2)   */
    seq_t a2 = (seq_t)57;  /* a(3)   */
    seq_t a3 = (seq_t)158; /* a(4)   */

    seq_t next;
    for (idx_t i = 5; i <= n; ++i)
    {
        next = (seq_t)4 * a3 - (seq_t)2 * a2 - (seq_t)4 * a1 - a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return a3;
}
