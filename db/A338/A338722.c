/*
 * A338722  Row sums in triangle A338721.
 * https://oeis.org/A338722
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..K} (2*floor((n - k(k-1)/2)/k) - 1), K = max k
 *            with k(k+1)/2 <= n (row n of A338721)
 * Range:     u8 n <= 54, u16 n <= 6786, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 74 terms through n = 9250 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 1; k * (k + 1) / 2 <= n; ++k) {
        idx_t m = (n - k * (k - 1) / 2) / k;
        sum += (seq_t)2 * (seq_t)m - 1;
    }
    return sum;
}
