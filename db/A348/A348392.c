/*
 * A348392  Row sums of the irregular triangle A348389.
 * https://oeis.org/A348392
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A143127(n) - n*(n+1)/2
 * Range:     u8 n <= 14, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A143127(n) is the sum of all multiples of k ≤ n for k=1..n.
   For each k, the sum of its multiples ≤ n is k * m * (m+1) / 2
   where m = floor(n / k). */

seq_t a(idx_t n) {
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        idx_t m = n / k;
        seq_t term = (seq_t)k * (seq_t)m * ((seq_t)m + 1) / 2;
        sum += term;
    }
    seq_t tri = (seq_t)n * ((seq_t)n + 1) / 2;
    return sum - tri;
}
