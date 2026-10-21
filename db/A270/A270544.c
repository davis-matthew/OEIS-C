/*
 * A270544  Number of ordered pairs (i,j) with |i|, |j| <= n, |i * j| <= n,
 *          and i odd.
 * https://oeis.org/A270544
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*floor((n+1)/2) +
 *            4*Sum_{k=0..floor((n+1)/2)-1}floor(n/(2k+1))
 * Range:     u8 n <= 26, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute m = floor((n+1)/2) without overflow */
    idx_t m = (n >> 1) + ((n & 1) ? 1 : 0);

    seq_t sum = 0;
    for (idx_t k = 0; k < m; ++k)
    {
        idx_t denom = 2 * k + 1;        /* odd denominators */
        seq_t term  = (seq_t)(n / denom);
        sum += term;
    }

    seq_t result = 2 * (seq_t)m + 4 * sum;
    return result;
}
