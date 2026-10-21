/*
 * A276489  a(n) = 25^(n+1)*Gamma(n+8/5)/Gamma(3/5).
 * https://oeis.org/A276489
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*(5*n+3)*a(n-1), a(0) = 15 This implements the product
 *            form: a(n) = ∏_{k=0..n} 5*(5*k+3)
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 15;   /* a(0) */
    for (idx_t k = 1; k <= n; ++k)
    {
        seq_t factor = (seq_t)5 * ( (seq_t)k * (seq_t)5 + (seq_t)3 );
        result = result * factor;
    }
    return result;
}
