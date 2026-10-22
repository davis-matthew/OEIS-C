/*
 * A345457  a(n) = Sum_{k=0..n} binomial(5*n+3,5*k).
 * https://oeis.org/A345457
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(5*n+3,5*k) * Recurrence: a(0)=1,
 *            a(1)=57, a(2)=1574, * a(n)=21*a(n-1)+353*a(n-2)-32*a(n-3) for
 *            n>2
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)57;
    if (n == 2) return (seq_t)1574;

    seq_t a0 = (seq_t)1;         /* a(0) */
    seq_t a1 = (seq_t)57;        /* a(1) */
    seq_t a2 = (seq_t)1574;      /* a(2) */
    seq_t a_next;

    for (idx_t i = 3; i <= n; ++i) {
        a_next = 21 * a2 + 353 * a1 - 32 * a0;
        a0 = a1;
        a1 = a2;
        a2 = a_next;
    }
    return a2;
}
