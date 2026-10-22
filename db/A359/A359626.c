/*
 * A359626  a(n) is equal to the number of filled unit triangles in a regular
 *          triangle whose coloring scheme is given in the comments.
 * https://oeis.org/A359626
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+3)/2 - 1 if n mod 6 = 1 or 2, a(n) = n*(n+3)/2 if n
 *            mod 6 = 0 or 3, a(n) = n*(n+3)/2 + 1 if n mod 6 = 4 or 5
 * Range:     u8 n <= 21, u16 n <= 360, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t r = n % 6;
    seq_t nn = (seq_t)n;
    seq_t base = nn * (nn + (seq_t)3) / (seq_t)2;
    if (r == 1 || r == 2) {
        return base - (seq_t)1;
    } else if (r == 4 || r == 5) {
        return base + (seq_t)1;
    } else {
        return base;
    }
}
