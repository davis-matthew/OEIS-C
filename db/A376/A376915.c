/*
 * A376915  Minimum number of edge pieces in a rectangular jigsaw puzzle with
 *          n pieces.
 * https://oeis.org/A376915
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = minimum over all factor pairs (d, n/d) of * the number of
 *            edge pieces: * if d==1 && n/d==1 -> 1 * else if d==1 -> n/d *
 *            else if n/d==1 -> d * else -> 2*d + 2*(n/d) - 4 * This is a
 *            direct evaluation over the divisors of n
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* not used for the offset 1 */
    seq_t min_edge = (seq_t)n;     /* worst case: all pieces on an edge */

    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            idx_t e = n / d;      /* complementary divisor */

            seq_t val;
            if (d == 1 && e == 1) {
                val = 1;
            } else if (d == 1) {
                val = (seq_t)e;
            } else if (e == 1) {
                val = (seq_t)d;
            } else {
                val = (seq_t)2 * (seq_t)d + (seq_t)2 * (seq_t)e - (seq_t)4;
            }

            if (val < min_edge) min_edge = val;
        }
    }

    return min_edge;
}
