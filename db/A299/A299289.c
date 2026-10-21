/*
 * A299289  Coordination sequence for "tsi" 3D uniform tiling.
 * https://oeis.org/A299289
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; for n>0, if n even a(n)=(13*n^2+4)/2 else
 *            a(n)=(13*n^2+3)/2
 * Range:     u8 n <= 4, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    seq_t nn = (seq_t)n;
    seq_t val = (seq_t)13 * nn * nn;
    if (n & 1)
        val += (seq_t)3;
    else
        val += (seq_t)4;
    val /= (seq_t)2;
    return val;
}
