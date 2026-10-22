/*
 * A363710  a(n) is the number of pairs of nonnegative integers (x, y) such
 *          that x + y = n and A003188(x) AND A003188(y) = 0 (where AND
 *          denotes the bitwise AND operator).
 * https://oeis.org/A363710
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = #{(x, y): x + y = n, (x^(x>>1)) & (y^(y>>1)) = 0}
 * Range:     u8 n <= 884, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t count = 0;
    for (idx_t x = 0; x <= n; ++x) {
        idx_t y = n - x;
        seq_t gx = (seq_t)x ^ ((seq_t)x >> 1);
        seq_t gy = (seq_t)y ^ ((seq_t)y >> 1);
        if ((gx & gy) == (seq_t)0) {
            ++count;
        }
    }
    return count;
}
