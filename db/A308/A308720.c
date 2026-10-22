/*
 * A308720  The maximum value in the continued fraction of sqrt(n), or 0 if
 *          there is no fractional part.
 * https://oeis.org/A308720
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 0 for perfect squares; otherwise 2*floor(sqrt(n))
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Integer square root for 64‑bit unsigned integers. */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t low  = 0;
    idx_t high = (idx_t)1 << 32; /* sqrt(UINT64_MAX) < 2^32 */

    while (low < high) {
        idx_t mid = (low + high) >> 1;
        idx_t sq  = mid * mid;
        if (sq <= n)
            low = mid + 1;
        else
            high = mid;
    }
    return low - 1; /* floor(sqrt(n)) */
}

seq_t a(idx_t n)
{
    idx_t r = isqrt_u64(n);
    if (r * r == n)      /* perfect square */
        return (seq_t)0;
    return (seq_t)(2 * r);
}
