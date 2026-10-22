/*
 * A367956  a(n) = Product_{i=1..n, j=1..n} (i + 3*j).
 * https://oeis.org/A367956
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{i=1..n, j=1..n} (i + 3*j)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t prod = (seq_t)1;
    for (idx_t i = (idx_t)1; i <= n; ++i) {
        for (idx_t j = (idx_t)1; j <= n; ++j) {
            prod *= ((seq_t)i + ((seq_t)3) * j);
        }
    }
    return prod;
}
