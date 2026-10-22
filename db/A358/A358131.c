/*
 * A358131  Triangle T(n,k) read by rows, where each row lists the value of n
 *          coins, in cents, using k dimes (10 cents) and n-k quarters (25
 *          cents).
 * https://oeis.org/A358131
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = 10*k + 25*(m - k), where m = floor((sqrt(1 + 8*n) -
 *            1)/2), and k = n - m*(m + 1)/2 (0 ≤ k ≤ m)
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline unsigned __int128 tri(idx_t x) {
    return (unsigned __int128)x * (x + 1) / 2;
}

static inline idx_t find_row(idx_t n) {
    idx_t lo = 0, hi = 1;
    /* double upper bound until it exceeds n */
    while (tri(hi) <= n) hi <<= 1;
    /* binary search for largest m with tri(m) ≤ n */
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if (tri(mid) <= n)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n) {
    /* Determine row m and column k */
    idx_t m = find_row(n);
    idx_t k = n - (idx_t)tri(m);
    /* Compute value */
    return (seq_t)10 * (seq_t)k + (seq_t)25 * (seq_t)(m - k);
}
