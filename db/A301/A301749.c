/*
 * A301749  Clique covering number of the n-odd graph.
 * https://oeis.org/A301749
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = ceil(binomial(2n-1, n-1)/2) for n≥3; a(1)=a(2)=1
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n <= 2) return 1;
    seq_t m = (seq_t)n * 2 - 1;          /* 2n-1 */
    seq_t k = (seq_t)n - 1;              /* n-1 */
    seq_t bin = 1;
    for (idx_t i = 1, lim = (idx_t)k; i <= lim; ++i) {
        bin *= (m - k + (seq_t)i);      /* multiply by (m-k+i) */
        bin /= (seq_t)i;                /* divide by i (exact division) */
    }
    seq_t res = bin / 2;                /* floor division by 2 */
    if (bin & 1) ++res;                 /* add 1 if bin is odd, i.e. ceil */
    return res;
}
