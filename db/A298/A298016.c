/*
 * A298016  Coordination sequence of snub-632 tiling with respect to a
 *          hexavalent node.
 * https://oeis.org/A298016
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   For n>=1, let k = floor(n/3). Then * a(3k) = 12k, * a(3k+1) =
 *            18k+6, * a(3k+2) = 24k+12; * a(0) = 1
 * Range:     u8 n <= 34, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }
    idx_t k = n / 3;
    idx_t r = n % 3;
    if (r == 0) {
        return (seq_t)12 * (seq_t)k;
    }
    if (r == 1) {
        return (seq_t)18 * (seq_t)k + (seq_t)6;
    }
    /* r == 2 */
    return (seq_t)24 * (seq_t)k + (seq_t)12;
}
