/*
 * A290268  Number of terms in the fully expanded n-th derivative of x^(x^2).
 * https://oeis.org/A290268
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n^2/2) + n + 1 - ((n mod 2) * floor((n + 1)/8))
 * Range:     u8 n <= 21, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tn = (seq_t)n;
    seq_t result = (tn * tn) / 2;       /* floor(n^2/2) */
    result += tn + 1;                   /* + n + 1 */
    if (n & 1)                          /* if n is odd */
        result -= (tn + 1) / 8;         /* subtract floor((n+1)/8) */
    return result;
}
