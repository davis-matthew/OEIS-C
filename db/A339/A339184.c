/*
 * A339184  Number of partitions of n into two parts such that the larger part
 *          is a nonzero square.
 * https://oeis.org/A339184
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = # of nonzero squares s² with ceil(n/2) ≤ s² ≤ n-1 (i.e.,
 *            number of partitions of n into two positive parts whose larger
 *            part is a nonzero perfect square)
 * Range:     u8 n <= 92, u16 n <= 92, u32 n <= 92, u64 n <= 92, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline idx_t isqrt_u64(idx_t n) {
    idx_t r = 0;
    for (idx_t bit = (idx_t)1ULL << 32; bit; bit >>= 1) {
        idx_t next = r + bit;
        if ((__uint128_t)next * next <= n)
            r = next;
    }
    return r;
}

seq_t a(idx_t n) {
    if (n < 2)
        return 0;                                     /* no 2‑part partitions */
    idx_t max_k = isqrt_u64(n - 1);                    /* largest s with s² ≤ n-1 */
    idx_t lower = (n + 1) / 2;                         /* ceil(n/2) */
    idx_t min_k = isqrt_u64(lower - 1);                /* largest s with s² < lower */
    return (seq_t)(max_k - min_k);                    /* count of squares in range */
}
