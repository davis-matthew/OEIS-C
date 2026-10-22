/*
 * A339919  a(n) = Sum_{k=1..n} (floor(3*n/k) - 3*floor(n/k)).
 * https://oeis.org/A339919
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1..n} (⌊3*n/k⌋ - 3*⌊n/k⌋)
 * Range:     u8 n <= 68, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t total = 0;
    seq_t N      = (seq_t)n;        /* cast index to the sequence type */
    seq_t N3     = N * 3;           /* 3*n, computed in seq_t to avoid overflow */

    for (idx_t k = 1; k <= n; ++k) {
        /* compute floor(3*n/k) and floor(n/k) using integer division */
        seq_t t1 = N3 / (seq_t)k;   /* floor(3*n/k) */
        seq_t t2 = N  / (seq_t)k;   /* floor(n/k)   */
        total += t1 - 3 * t2;
    }
    return total;
}
