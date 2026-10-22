/*
 * A376523  a(n) = Product_{k=0..n} (k^3 + n - k).
 * https://oeis.org/A376523
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k=0..n} (k^3 + n - k)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t prod = 1;
    for (idx_t k = 0; k <= n; ++k) {
        seq_t ksq = (seq_t)k * k;          /* k^2 */
        seq_t kcu = ksq * k;               /* k^3 */
        seq_t term = kcu + (seq_t)n - (seq_t)k;
        prod *= term;
    }
    return prod;
}
