/*
 * A267322  Expansion of (1 + x + x^2 + x^4 + 2*x^5)/(1 - x^3)^3.
 * https://oeis.org/A267322
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (floor(n/3)+1)*((n+1)*floor(n/3)-3*floor(n/3)^2+2)/2
 * Range:     u8 n <= 40, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t k = n / 3;                                 /* floor(n/3) */
    seq_t term = ((seq_t)(n + 1)) * k                /* (n+1)*k */
               - (seq_t)3 * k * k                    /*  - 3*k^2 */
               + 2;                                 /* + 2 */
    return ((k + 1) * term) / 2;                     /* result */
}
