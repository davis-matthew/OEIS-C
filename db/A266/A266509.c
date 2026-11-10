/*
 * A266509  Terms of A256263 repeated.
 * https://oeis.org/A266509
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = A256263(floor((n-1)/2)), where A256263 (offset 0) is the
 *            irregular triangle with row j>=1 of length 2^(j-1), T(j,k) =
 *            2k-1 if k is a power of 2, else 6*(k-2^m)-1 with m =
 *            floor(log2(k)), and A256263(0) = 0
 * Range:     u8 n <= 468, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Validated: standard, pass, 106 terms through n = 7410 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline int A256263_floor_log2_idx(idx_t v)
{
    int r = 0;
    while (v >>= 1) r++;
    return r;
}

static seq_t A256263_a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;
    int j = A256263_floor_log2_idx(n) + 1;
    idx_t start = (idx_t)1 << (j - 1);
    idx_t k = n - start + 1;
    if ((k & (k - 1)) == 0) {
        return (seq_t)k * 2 - 1;
    } else {
        int m = A256263_floor_log2_idx(k);
        idx_t base = (idx_t)1 << m;
        idx_t offset = k - base;
        return (seq_t)offset * 6 - 1;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    return A256263_a((n - 1) / 2);
}
