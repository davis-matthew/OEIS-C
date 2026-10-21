/*
 * A258098  3x + 1 sequence starting at 79.
 * https://oeis.org/A258098
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)*3+1 if a(n-1) is odd, else a(n-1)/2 a(0) = 79
 * Range:     u8 n <= 2, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val = (seq_t)79;
    for (idx_t i = 1; i <= n; ++i) {
        if (val % (seq_t)2 != (seq_t)0)
            val = val * (seq_t)3 + (seq_t)1; /* 3*val + 1 */
        else
            val = val / (seq_t)2;           /* val / 2      */
    }
    return val;
}
