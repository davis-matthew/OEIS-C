/*
 * A371515  Centered truncated octahedral numbers: the number of integer
 *          triples (x,y,z) such that max(|x|,|y|,|z|) <= 2n and |x|+|y|+|z|
 *          <= 3n.
 * https://oeis.org/A371515
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 32*n^3 + 18*n^2 + 6*n + 1
 * Range:     u8 n <= 1, u16 n <= 12, u32 n <= 511, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn  = (seq_t)n;          /* Convert index to seq_t */
    seq_t n2  = sn * sn;           /* n^2 */
    seq_t n3  = n2 * sn;           /* n^3 */
    seq_t val = (seq_t)32 * n3
              + (seq_t)18 * n2
              + (seq_t)6  * sn
              + (seq_t)1;
    return val;
}
