/*
 * A270444  Expansion of g.f. 2*(1 + 2*x)/(1 - 8*x + 4*x^2).
 * https://oeis.org/A270444
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-1) - 4*a(n-2) with a(1)=2, a(2)=20
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;                /* offset starts at 1 */
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)20;

    seq_t a_prev2 = (seq_t)2;   /* a(1) */
    seq_t a_prev1 = (seq_t)20;  /* a(2) */
    seq_t a_cur   = 0;

    for (idx_t i = 3; i <= n; ++i) {
        a_cur = (seq_t)8 * a_prev1 - (seq_t)4 * a_prev2;
        a_prev2 = a_prev1;
        a_prev1 = a_cur;
    }
    return a_cur;
}
