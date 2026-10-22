/*
 * A397444  Anion excess of a saturated rock-salt (NaCl) cube with n atoms
 *          along each edge: a(n) = 3*n^2 + 1 - (-1)^n.
 * https://oeis.org/A397444
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*n*n + 2*(n%2)
 * Range:     u8 n <= 9, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t res = nn * nn;
    res = res * (seq_t)3;
    if (n & 1) {
        res += (seq_t)2;
    }
    return res;
}
