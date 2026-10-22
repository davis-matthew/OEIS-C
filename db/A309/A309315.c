/*
 * A309315  Number of 5-colorings of an n-wheel graph.
 * https://oeis.org/A309315
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*3^(n-1)-15*(-1)^n
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 39, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(3)=60, a(4)=120, a(n)=2*a(n-1)+3*a(n-2) for n>4 */
seq_t a(idx_t n)
{
    if (n < 3)
        return 0;
    if (n == 3)
        return (seq_t)60;
    if (n == 4)
        return (seq_t)120;

    seq_t a_prev2 = (seq_t)60; /* a(3) */
    seq_t a_prev1 = (seq_t)120; /* a(4) */
    seq_t a_curr;

    for (idx_t i = 5; i <= n; ++i) {
        a_curr = a_prev1 * (seq_t)2 + a_prev2 * (seq_t)3;
        a_prev2 = a_prev1;
        a_prev1 = a_curr;
    }
    return a_prev1;
}
