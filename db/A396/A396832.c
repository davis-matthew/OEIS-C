/*
 * A396832  Number of permutations p of [n] such that k * p(k) >= n-1 for 1 <=
 *          k <= n.
 * https://oeis.org/A396832
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k=1..n} (n - k + 2 - ceil((n-1)/k)), a(0)=1,
 *            a(1)=1
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 14, u64 n <= 22, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0 || n == 1) return (seq_t)1;
    seq_t prod = (seq_t)1;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t ceil_val = ((seq_t)n + k - (seq_t)2) / k;           /* ceil((n-1)/k) */
        seq_t term = (seq_t)n - k + (seq_t)2 - ceil_val;          /* n - k + 2 - ceil */
        prod *= term;
    }
    return prod;
}
