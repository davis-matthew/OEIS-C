/*
 * A322042  Maximum edge-distance of a point in the quotient graph E/nE from
 *          the origin (see A322041 for further information).
 * https://oeis.org/A322042
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n - ceil(n/3) = n - ((n+2)/3)
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    seq_t nn = (seq_t)n;
    seq_t ceil_div = (nn + 2) / 3;
    return nn - ceil_div;
}
