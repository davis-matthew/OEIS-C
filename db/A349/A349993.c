/*
 * A349993  a(n) is the number of squares k^2 with n^2 <= k^2 <= n^3.
 * https://oeis.org/A349993
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(sqrt(n^3)) - n + 1
 * Range:     u8 n <= 44, u16 n <= 1652, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 64‑bit integer square root (floor).  Uses only integer arithmetic. */
static idx_t _isqrt(idx_t x)
{
    idx_t lo = 0, hi = ((idx_t)1u << 32);  /* hi > sqrt(max_uint64) */
    while (lo < hi)
    {
        /* Choose mid as the upper half to avoid infinite loop */
        idx_t mid = lo + ((hi - lo + 1) >> 1);
        /* Check for overflow in mid*mid by using division */
        if (mid == 0 || mid <= x / mid)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    /* Use idx_t for all intermediate arithmetic to avoid overflow of
       seq_t when it is small (e.g. uint8_t).  seq_t is only used for the
       final cast. */
    idx_t cube = n * n * n;          /* n^3 fits in 64 bits for n ≤ 10000 */
    idx_t root = _isqrt(cube);       /* floor(sqrt(n^3)) */
    idx_t val  = root - n + (idx_t)1;
    return (seq_t)val;
}
