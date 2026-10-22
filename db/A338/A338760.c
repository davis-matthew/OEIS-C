/*
 * A338760  Subword complexity of the infinite word Product_{i>=1}
 *          Product_{j=1..i} a^(i-j+1) b^j.
 * https://oeis.org/A338760
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n for n <= 3, otherwise (n^3 + 3*n^2 - 10*n + 18)/6
 * Range:     u8 n <= 5, u16 n <= 39, u32 n <= 1811, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 3) {
        return ((seq_t)1 << (unsigned)n);
    }
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    seq_t n3 = n2 * nn;
    seq_t num = n3 + (seq_t)3 * n2 - (seq_t)10 * nn + (seq_t)18;
    return num / (seq_t)6;
}
