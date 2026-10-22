/*
 * A363341  Number of positive integers k <= n such that round(n/k) is odd.
 * https://oeis.org/A363341
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   round(n/k) = floor(n/k + 1/2) = (n + k/2)/k (integer arithmetic)
 * Range:     u8 n <= 69, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = #{ 1 ≤ k ≤ n  |  round(n/k) is odd } */

seq_t a(idx_t n)
{
    seq_t cnt = 0;
    for (idx_t k = 1; k <= n; ++k) {
        /* round(n/k) = floor(n/k + 1/2) = (n + k/2) / k     */
        seq_t r = ((seq_t)n + (seq_t)(k / 2)) / (seq_t)k;
        if (r & 1)
            ++cnt;
    }
    return cnt;
}
