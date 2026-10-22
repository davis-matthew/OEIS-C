/*
 * A344684  Sum of two consecutive products of Fibonacci and Pell numbers:
 *          F(n)*P(n) + F(n+1)*P(n+1).
 * https://oeis.org/A344684
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = F(n)*P(n) + F(n+1)*P(n+1) where F(n) is the Fibonacci
 *            number A000045(n) and P(n) is the Pell number A000129(n). The
 *            recurrence for Pell is P(n) = 2*P(n-1)+P(n-2), with P(0)=0,
 *            P(1)=1
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Start with F(0)=0, F(1)=1 and P(0)=0, P(1)=1 */
    seq_t f0 = 0, f1 = 1;   /* f0 = F(k), f1 = F(k+1) */
    seq_t p0 = 0, p1 = 1;   /* p0 = P(k), p1 = P(k+1) */

    /* Advance the sequences n times so that after the loop
       f0 = F(n), f1 = F(n+1), p0 = P(n), p1 = P(n+1) */
    for (idx_t i = 0; i < n; ++i) {
        seq_t f2 = f0 + f1;                   /* F(k+2) */
        f0 = f1;
        f1 = f2;
        seq_t p2 = p0 + ((seq_t)2 * p1);      /* P(k+2) = 2*P(k+1)+P(k) */
        p0 = p1;
        p1 = p2;
    }

    /* Compute the required combination */
    return f0 * p0 + f1 * p1;
}
