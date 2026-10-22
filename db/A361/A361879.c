/*
 * A361879  Sum of even middle divisors of n, where "middle divisor" means a
 *          divisor in the half-open interval [sqrt(n/2), sqrt(n*2)).
 * https://oeis.org/A361879
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = sum of even divisors d of n such that sqrt(n/2) <= d <
 *            sqrt(2n)
 * Range:     u8 n <= 2519, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes floor(sqrt(x)) for an unsigned 64‑bit integer.
   Uses binary search with 128‑bit intermediate type to avoid overflow. */
static inline idx_t isqrt_u64(idx_t x)
{
    if (x == 0) return 0;
    idx_t lo = 1, hi = x;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq <= x)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;
}

seq_t a(idx_t n)
{
    /* n >= 1 by definition; lower bound for n = 0 simply returns 0. */
    if (n == 0) return 0;

    /* Lower limit: ceil(sqrt(n/2)) = ceil(sqrt(ceil(n/2))) */
    idx_t ceil_half = (n + 1) >> 1;           /* ceil(n/2) */
    idx_t low = isqrt_u64(ceil_half);
    if ((__uint128_t)low * low < ceil_half) low++;

    /* Upper limit: floor(sqrt(2n-1)) ensures d < sqrt(2n) */
    idx_t high = isqrt_u64((n << 1) - 1);

    seq_t sum = 0;
    for (idx_t d = low; d <= high; ++d) {
        if ((n % d) == 0 && ((d & 1) == 0))
            sum += (seq_t)d;
    }
    return sum;
}
