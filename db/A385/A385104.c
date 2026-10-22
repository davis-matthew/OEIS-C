/*
 * A385104  Triangle read by rows: T(n,k) is the number of residue classes
 *          obtained by solving mod(x^2,n) = k for x over the integers, n >=
 *          1, k >= 0.
 * https://oeis.org/A385104
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = number of solutions x mod r of x^2 ≡ k (mod r) where r
 *            and k are determined by flattening the triangle: r*(r-1)/2 < n ≤
 *            r*(r+1)/2, k = n - r*(r-1)/2 - 1
 * Range:     u8 n <= 9838, u16 n <= 9838, u32 n <= 9838, u64 n <= 9838,
 *            u128 n <= 9838, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n) {
    if (n == 0) return 0; /* handles index below offset, harmless */
    /* Find row r such that 1 + 2 + ... + r >= n */
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = (low + high) / 2;
        idx_t sum = (mid * (mid + 1)) / 2;
        if (sum < n)
            low = mid + 1;
        else
            high = mid;
    }
    idx_t r = low;
    idx_t sum_before = ((r - 1) * r) / 2;
    idx_t k = n - sum_before - 1; /* zero‑based column index */

    seq_t count = 0;
    seq_t mod = (seq_t)r;
    for (idx_t x = 0; x < r; ++x) {
        seq_t sq = ((seq_t)x * (seq_t)x) % mod;
        if (sq == (seq_t)k) ++count;
    }
    return count;
}
