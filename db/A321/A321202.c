/*
 * A321202  Row sums of the irregular triangle A321201.
 * https://oeis.org/A321202
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j≡n (mod 2), 0≤j≤⌊n/3⌋} (n−j)/2 (derived from
 *            2i+3j=n, i=(n−3j)/2, i+j=(n−j)/2)
 * Range:     u8 n <= 59, u16 n <= 971, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2) return 0;            /* offset is 2; values below are undefined */

    idx_t j0 = n & 1u;               /* j must have the same parity as n      */
    idx_t Jmax = n / 3u;             /* largest possible j                     */
    if (Jmax < j0) return 0;         /* no solutions for very small n (not needed) */

    /* number of terms m = floor((Jmax−j0)/2)+1 */
    idx_t t = Jmax - j0;
    idx_t m = (t / 2u) + 1u;

    /* a(n) = m*(n-j0)/2 − (m−1)*m/2   */
    seq_t term1 = (seq_t)m * ((seq_t)(n - j0)) / 2u;
    seq_t term2 = (seq_t)(m - 1u) * (seq_t)m / 2u;

    return term1 - term2;
}
