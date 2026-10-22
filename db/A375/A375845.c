/*
 * A375845  a(n) = Product_{k=0..n} (k^8 + n).
 * https://oeis.org/A375845
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k=0..n} (k^8 + n)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t prod = (seq_t)1;
    for (idx_t k = (idx_t)0; k <= n; ++k)
    {
        seq_t kk  = (seq_t)k;
        seq_t k2  = kk * kk;        // k^2
        seq_t k4  = k2 * k2;        // k^4
        seq_t k8  = k4 * k4;        // k^8
        seq_t term = k8 + (seq_t)n; // k^8 + n
        prod *= term;
    }
    return prod;
}
