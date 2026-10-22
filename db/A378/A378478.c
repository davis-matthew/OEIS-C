/*
 * A378478  The number of n-colorings of the vertices of the snub dodecahedron
 *          up to rotation.
 * https://oeis.org/A378478
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1/60*(n^60 + 15*n^30 + 20*n^20 + 24*n^12)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 1, u64 n <= 2, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow_seq(seq_t base, unsigned int exp)
{
    seq_t result = (seq_t)1;
    while (exp)
    {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    seq_t n12 = pow_seq(N, 12);
    seq_t n20 = pow_seq(N, 20);
    seq_t n30 = pow_seq(N, 30);
    seq_t n60 = pow_seq(N, 60);

    seq_t sum = n60 + n30 * (seq_t)15 + n20 * (seq_t)20 + n12 * (seq_t)24;
    return sum / (seq_t)60;
}
