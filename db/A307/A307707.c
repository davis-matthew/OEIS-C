/*
 * A307707  Lexicographically earliest sequence of nonnegative integers in
 *          which, for all k >= 0, there are exactly k pairs of consecutive
 *          terms whose sum is k.
 * https://oeis.org/A307707
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   let t_m = m*(m+1)/2. For n with t_{m-1} < n ≤ t_m and j = n -
 *            t_{m-1} - 1, a(n) = { m/2 if m even (m-1)/2 + (j mod 2) if m odd
 *            }
 * Range:     u8 n <= 7832, u16 n <= 7832, u32 n <= 7832, u64 n <= 7832,
 *            u128 n <= 7832, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Integer square‑root (floor) for 64‑bit unsigned integers. */
static inline idx_t floor_sqrt(idx_t n) {
    idx_t x = n;
    if (x == 0) return 0;
    idx_t y = (x + 1) >> 1;
    while (y < x) {
        x = y;
        y = (x + n / x) >> 1;
    }
    return x;
}

seq_t a(idx_t n) {
    /* Find block index m so that t_{m-1} < n ≤ t_m. */
    idx_t s   = floor_sqrt((idx_t)1 + (idx_t)8 * n);
    idx_t m   = (s - 1) / 2;
    idx_t t_m = (m * (m + 1)) / 2;
    if (t_m < n) ++m;                 /* ceil to get correct block */

    /* Starting index of this block. */
    idx_t start = ((m - 1) * m) / 2 + 1;
    idx_t j = n - start;              /* 0‑based offset inside the block */

    if ((m & 1) == 0) {               /* even block: all values are m/2 */
        return (seq_t)(m / 2);
    } else {                          /* odd block: alternating values */
        seq_t lower = (seq_t)((m - 1) / 2);
        return (seq_t)(lower + ( (j & 1) ? 1 : 0 ));
    }
}
