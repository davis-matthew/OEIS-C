/*
 * A253679  Numbers that begin a run of an odd number of consecutive integers
 *          whose cubes sum to a square.
 * https://oeis.org/A253679
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2n+1)^3 - (3n+1)
 * Range:     u8 n <= 2, u16 n <= 19, u32 n <= 812, u64 n <= 9442,
 *            u128 n <= 9442, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* below the offset */
    seq_t nn = (seq_t)n;
    seq_t m  = 2*nn + 1;
    seq_t m2 = m * m;
    seq_t m3 = m2 * m;
    seq_t res = m3 - (3*nn + 1);
    return res;
}
