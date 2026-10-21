/*
 * A299412  Pentagonal pyramidal numbers divisible by 3.
 * https://oeis.org/A299412
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((3*n + (n%2))/2)^2 * (((3*n + (n%2))/2)+1)/2
 * Range:     u8 n <= 4, u16 n <= 33, u32 n <= 1363, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* t = (3*n + (n%2)) / 2  (always an integer) */
    seq_t t = ((seq_t)3 * (seq_t)n + (seq_t)(n & 1)) / 2;
    return t * t * (t + 1) / 2;
}
