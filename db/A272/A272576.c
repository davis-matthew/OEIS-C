/*
 * A272576  a(n) = f(10, f(9, n)), where f(k,m) = floor(m*k/(k-1)).
 * https://oeis.org/A272576
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = floor( floor(n*9/8) * 10/9 )
 * Range:     u8 n <= 205, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* First apply f(9, n) = floor(n*9/8) */
    seq_t m = ((seq_t)n * (seq_t)9) / (seq_t)8;
    /* Then apply f(10, m) = floor(m*10/9) */
    return (m * (seq_t)10) / (seq_t)9;
}
