/*
 * A368067  a(n) = Product_{i=1..n, j=1..n} (i^2 + 3*i*j + j^2).
 * https://oeis.org/A368067
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{i=1..n} ∏_{j=1..n} (i^2 + 3*i*j + j^2)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 2, u64 n <= 3, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t prod = 1;
    for (idx_t i = 1; i <= n; ++i) {
        for (idx_t j = 1; j <= n; ++j) {
            seq_t ii = (seq_t)i * i;
            seq_t jj = (seq_t)j * j;
            seq_t ij = (seq_t)3 * i * j;
            seq_t val = ii + ij + jj;
            prod *= val;
        }
    }
    return prod;
}
