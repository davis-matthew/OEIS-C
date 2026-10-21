/*
 * A271723  Numbers k such that 3*k - 8 is a square.
 * https://oeis.org/A271723
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (6*n^2 - 8*n + 24)/8 if n is even, a(n) = (6*n^2 - 4*n +
 *            22)/8 if n is odd. This equals (6*(n-1)*n - (2*n-1)*(-1)^n +
 *            23)/8
 * Range:     u8 n <= 18, u16 n <= 296, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t t = (seq_t)n;
    seq_t val;
    if (n & 1) {
        val = ((seq_t)6 * t * t - (seq_t)4 * t + (seq_t)22) / (seq_t)8;
    } else {
        val = ((seq_t)6 * t * t - (seq_t)8 * t + (seq_t)24) / (seq_t)8;
    }
    return val;
}
