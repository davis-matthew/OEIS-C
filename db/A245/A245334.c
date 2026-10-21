/*
 * A245334  A factorial-like triangle read by rows: T(0,0) = 1; T(n+1,0) =
 *          T(n,0)+1; T(n+1,k+1) = T(n,0)*T(n,k), k=0..n.
 * https://oeis.org/A245334
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (row+1-k) * ∏_{i=0}^{k-1} (row-i) where row =
 *            ⌊(⌊√(8n+1)⌋-1)/2⌋ k = n - row(row+1)/2 The triangle T is defined
 *            by T(0,0) = 1; T(n+1,0) = T(n,0) + 1; T(n+1,k+1) = T(n,0) *
 *            T(n,k) (0 ≤ k ≤ n)
 * Range:     u8 n <= 23, u16 n <= 42, u32 n <= 101, u64 n <= 248,
 *            u128 n <= 626, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static idx_t
isqrt_u64(idx_t v)
{
    idx_t low = 0, high = v;
    while (low <= high) {
        idx_t mid = (low + high) >> 1;
        if (mid == 0) {
            low = mid + 1;
            continue;
        }
        if (mid <= v / mid) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return high;               /* floor(sqrt(v)) */
}

seq_t a(idx_t n)
{
    /* Find the row that contains index n */
    idx_t row = (isqrt_u64(8*n + 1) - 1) / 2;
    /* Adjust in case of rounding errors */
    while ((row + 1) * (row + 2) / 2 <= n) ++row;
    while (row * (row + 1) / 2 > n) --row;

    /* Column position within the row */
    idx_t start = row * (row + 1) / 2;
    idx_t k = n - start;

    /* Compute T(row,k) = (row+1-k) * ∏_{i=0}^{k-1} (row-i) */
    seq_t value = (seq_t)(row + 1 - k);
    for (idx_t i = 0; i < k; ++i) {
        value *= (seq_t)(row - i);
    }
    return value;
}
