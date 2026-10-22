/*
 * A389111  Write 0,1,2,3,4,... in a triangular spiral, a(n) is such that n
 *          lies geometrically half way between 0 and a(n).
 * https://oeis.org/A389111
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n + 9*round(sqrt(2*(n+1))/3)^2
 * Range:     u8 n <= 54, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Integer square‑root for 64‑bit values.
 * Uses binary search with 128‑bit arithmetic to avoid overflow. */
static inline idx_t int_sqrt_u64(idx_t n)
{
    idx_t low  = 0;
    idx_t high = ((idx_t)1ULL << 32);   /* sqrt(2^64-1) < 2^32 */
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq <= n)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;
}

seq_t a(idx_t n)
{
    /* m = 2 * (n + 1) */
    idx_t m = 2 * (n + 1);

    /* s = floor( sqrt(m) ) */
    idx_t s = int_sqrt_u64(m);

    /* initial k = floor( s / 3 ) */
    idx_t k = s / 3;

    /* adjust k if rounding threshold is crossed:
     * round(x) = floor(x + 0.5)
     * For x = sqrt(m)/3, we round up when sqrt(m) >= 3*k + 1.5
     * This equivalent to m >= 9*k^2 + 9*k + 3. */
    if (m >= (idx_t)(9ULL * k * k) + (idx_t)(9ULL * k) + (idx_t)3)
        ++k;

    /* compute the result: 2*n + 9*k^2 */
    seq_t tn = (seq_t)n;
    seq_t kk = (seq_t)k;
    seq_t ans = (seq_t)2 * tn + (seq_t)9 * (kk * kk);
    return ans;
}
