/*
 * A249348  a(n) = (A001147(n+1)^2-1)/8, where A001147(n+1) = 3*5*...*(2n+1).
 * https://oeis.org/A249348
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=0, a(1)=1, and for n≥2
 *            (-n+1)*a(n)+2*n*(2*n^2-1)*a(n-1)-(n+1)*(2*n-1)^2*a(n-2)=0
 *            Equivalently a(n)=
 *            [2*n*(2*n^2-1)*a(n-1)-(n+1)*(2*n-1)^2*a(n-2)]/(n-1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    seq_t prev2 = (seq_t)0; /* a[0] */
    seq_t prev1 = (seq_t)1; /* a[1] */
    seq_t cur  = 0;

    for (idx_t i = 2; i <= n; ++i)
    {
        /* i^2 as seq_t */
        seq_t ii = (seq_t)i * (seq_t)i;

        /* coeff1 = 2*i * (2*i^2 - 1) */
        seq_t coeff1 = (seq_t)2 * (seq_t)i;
        coeff1 *= ((seq_t)2 * ii - (seq_t)1);

        /* coeff2 = (i+1) * (2*i-1)^2 */
        seq_t two_i_minus1 = ((seq_t)2 * (seq_t)i) - (seq_t)1;
        seq_t coeff2 = (seq_t)(i + 1);
        coeff2 *= two_i_minus1 * two_i_minus1;

        seq_t term1 = coeff1 * prev1;
        seq_t term2 = coeff2 * prev2;

        seq_t numerator = term1 - term2;
        seq_t denom     = (seq_t)i - (seq_t)1;
        cur = numerator / denom;

        /* shift for next iteration */
        prev2 = prev1;
        prev1 = cur;
    }

    return cur;
}
