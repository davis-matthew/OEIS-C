/*
 * A384243  a(n) = 2^(n-6)*n*(n^3 - 6*n^2 + 19*n - 14).
 * https://oeis.org/A384243
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n-6)*n*(n^3 - 6*n^2 + 19*n - 14)
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 20, u64 n <= 47, u128 n <= 107,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn   = (seq_t)n;
    seq_t poly = nn*nn*nn - (seq_t)6*nn*nn + (seq_t)19*nn - (seq_t)14;
    seq_t prod = nn * poly;

    if (n >= 6) {
        seq_t pow2 = 1;
        idx_t e = n - 6;
        for (idx_t i = 0; i < e; ++i) pow2 *= (seq_t)2;
        return prod * pow2;
    } else {
        seq_t pow2 = 1;
        idx_t e = 6 - n;
        for (idx_t i = 0; i < e; ++i) pow2 *= (seq_t)2;
        return prod / pow2;
    }
}
