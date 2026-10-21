/*
 * A271491  Arises in enumeration of locally convex functions.
 * https://oeis.org/A271491
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 (n=0), 3 (n=1), 9 (n=2), 16 (n=3), 20 (n=4), 21 (n>=5)
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)1;
    if (n == 1u) return (seq_t)3;
    if (n == 2u) return (seq_t)9;
    if (n == 3u) return (seq_t)16;
    if (n == 4u) return (seq_t)20;
    return (seq_t)21;
}
