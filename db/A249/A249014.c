/*
 * A249014  A double binomial sum.
 * https://oeis.org/A249014
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(0) = 1
   a(1) = 3
   a(2) = 15
   a(3) = 105
   a(n+4) = (4*n+15)*a(n+3)
            - 6*(n+3)^2*a(n+2)
            + 2*(n+3)*(n+2)*(2*n+5)*a(n+1)
            - (n+3)*(n+2)^2*(n+1)*a(n)
*/
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)15;
    if (n == 3) return (seq_t)105;

    seq_t a0 = (seq_t)1;   /* a(n-4)   */
    seq_t a1 = (seq_t)3;   /* a(n-3)   */
    seq_t a2 = (seq_t)15;  /* a(n-2)   */
    seq_t a3 = (seq_t)105; /* a(n-1)   */
    seq_t a_curr = (seq_t)0;

    for (idx_t i = 4; i <= n; ++i)
    {
        idx_t m = i - 4; /* n in the recurrence,  m = 0,1,2,... */

        seq_t coeff1 = (seq_t)(4*m + 15);
        seq_t p3 = (seq_t)(m + 3);
        seq_t p2 = (seq_t)(m + 2);
        seq_t p1 = (seq_t)(m + 1);

        seq_t term1 = coeff1 * a3;
        seq_t term2 = (seq_t)6 * p3 * p3 * a2;
        seq_t term3 = (seq_t)2 * p3 * p2 * (seq_t)(2*m + 5) * a1;
        seq_t term4 = p3 * p2 * p2 * p1 * a0;

        a_curr = term1 - term2 + term3 - term4;

        /* shift the window for next iteration */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a_curr;
    }
    return a_curr;
}
