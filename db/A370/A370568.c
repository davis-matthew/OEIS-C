/*
 * A370568  Expansion of g.f. (1-x) / (1-9*x+28*x^2-35*x^3+15*x^4-x^5).
 * https://oeis.org/A370568
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*a(n-1)-28*a(n-2)+35*a(n-3)-15*a(n-4)+a(n-5) for n>=5
 *            Initial values: a(0)=1, a(1)=8, a(2)=44, a(3)=207, a(4)=896
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 32, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle initial cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)8;
    if (n == 2) return (seq_t)44;
    if (n == 3) return (seq_t)207;
    if (n == 4) return (seq_t)896;

    /* iterative computation using the last five values */
    seq_t a0 = (seq_t)1,  a1 = (seq_t)8,  a2 = (seq_t)44,
          a3 = (seq_t)207, a4 = (seq_t)896, a5;

    for (idx_t i = 5; i <= n; ++i) {
        a5 = (seq_t)9 * a4 - (seq_t)28 * a3 + (seq_t)35 * a2
             - (seq_t)15 * a1 + a0;
        a0 = a1; a1 = a2; a2 = a3; a3 = a4; a4 = a5;
    }
    return a4;
}
