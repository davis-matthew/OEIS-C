/*
 * A344131  a(n+1) = (8*n^2+8*n+3)*a(n) - 16*n^4*a(n-1), with a(0)=0, a(1)=1.
 * https://oeis.org/A344131
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0; a(1) = 1; a(n+1) = (8*n^2+8*n+3)*a(n) - 16*n^4*a(n-1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    seq_t a_prev2 = (seq_t)0;   /* a(0) */
    seq_t a_prev1 = (seq_t)1;   /* a(1) */
    seq_t a_curr;

    for (idx_t i = 1; i < n; ++i)       /* compute a(i+1) from a(i) and a(i-1) */
    {
        /* compute coefficient 8*i^2 + 8*i + 3 */
        idx_t i2    = i * i;
        seq_t coeff = (seq_t)8 * (seq_t)i2 + (seq_t)8 * (seq_t)i + (seq_t)3;

        /* term1 = coeff * a(i) */
        seq_t term1 = a_prev1 * coeff;

        /* compute 16*i^4 */
        idx_t i4          = i2 * i2;
        seq_t mult_small  = (seq_t)16 * (seq_t)i4;   /* small integer < 2^64 */

        /* term2 = 16*i^4 * a(i-1) */
        seq_t term2 = a_prev2 * mult_small;

        a_curr = term1 - term2;
        a_prev2 = a_prev1;
        a_prev1 = a_curr;
    }
    return a_prev1;
}
