/*
 * A291300  Signed domination number of Fibonacci cube Gamma_n.
 * https://oeis.org/A291300
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(1)=2, a(2)=3, a(3)=3, a(4)=2, a(5)=5, a(6)=9, a(7)=10,
 *            a(n)=a(n-1)+a(n-2)-2 for all n >= 8 (OEIS A291300)
 * Range:     u8 n <= 10, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the first seven terms explicitly */
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)3;
    if (n == 3) return (seq_t)3;
    if (n == 4) return (seq_t)2;
    if (n == 5) return (seq_t)5;
    if (n == 6) return (seq_t)9;
    if (n == 7) return (seq_t)10;

    /* n >= 8: use the recurrence a(n)=a(n-1)+a(n-2)-2 */
    seq_t a_prev2 = (seq_t)9;  /* a(6) */
    seq_t a_prev1 = (seq_t)10; /* a(7) */
    seq_t result  = a_prev1;   /* placeholder */

    for (idx_t i = 8; i <= n; ++i)
    {
        result = a_prev1 + a_prev2 - (seq_t)2;
        a_prev2 = a_prev1;
        a_prev1 = result;
    }

    return result;
}
