/*
 * A375839  a(n) = Product_{k=0..n} (k^2 + n).
 * https://oeis.org/A375839
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prod_{k=0..n} (k^2 + n)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 11, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t prod = 1;
    idx_t k;
    for (k = 0; k <= n; ++k) {
        seq_t kk = (seq_t)k * (seq_t)k;
        prod *= kk + (seq_t)n;
    }
    return prod;
}
