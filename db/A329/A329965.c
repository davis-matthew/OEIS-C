/*
 * A329965  a(n) = ((1+n)*floor(1+n/2))*(n!/floor(1+n/2)!)^2.
 * https://oeis.org/A329965
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((1+n)*floor(1+n/2))*(n!/floor(1+n/2)!)^2
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* m = floor(1 + n/2) */
    idx_t m = n / 2 + 1;

    /* Compute n! / m! as product of integers (m+1)..n */
    seq_t prod = 1;
    for (idx_t i = m + 1; i <= n; ++i)
        prod *= (seq_t)i;

    /* Small factor (n+1) * m */
    seq_t small_factor = (seq_t)(n + 1) * (seq_t)m;

    /* a(n) = small_factor * prod^2 */
    seq_t result = prod;
    result *= prod;          /* prod^2 */
    result *= small_factor;  /* (n+1)*m*prod^2 */

    return result;
}
