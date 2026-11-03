/*
 * A354034  a(n) = 1 if n is an even number or a square, otherwise 0.
 * https://oeis.org/A354034
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n is even or a perfect square, else 0
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 138 terms through n = 9757 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline idx_t isqrt_u64(idx_t n) {
    /* integer floor square root for non‑negative n (idx_t is unsigned) */
    if (n < 2) return n;
    idx_t low = 1, high = n >> 1;
    idx_t ans = 1;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        idx_t div = n / mid;
        if (mid == div) return mid;
        if (mid < div) {
            ans = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return ans;
}
 
seq_t a(idx_t n) {
    /* Even numbers are always 1 */
    if ((n & 1u) == 0u)
        return (seq_t)1;
 
    /* Odd numbers: test perfect square */
    idx_t m = isqrt_u64(n);
    if ((seq_t)m * (seq_t)m == (seq_t)n)
        return (seq_t)1;
    return (seq_t)0;
}
