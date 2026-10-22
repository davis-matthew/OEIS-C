/*
 * A370481  a(0) = 33, a(n) = 3*a(n-1) + 2*n + 1 for n >= 1.
 * https://oeis.org/A370481
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + 2*n + 1; a(0) = 33
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 16, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val = (seq_t)33;
    for (idx_t i = 1; i <= n; ++i) {
        val = val * (seq_t)3;
        val = val + ((seq_t)2 * (seq_t)i) + (seq_t)1;
    }
    return val;
}
