/*
 * A293137  a(0) = 0, and a(n) = floor(2*sqrt(n)) - 1 for n >= 1.
 * https://oeis.org/A293137
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n==0) ? 0 : floor(2*sqrt(n)) - 1 * Computed by integer
 *            square‑root and a simple comparison
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline idx_t isqrt_uint64(idx_t n)
{
    idx_t lo = 0, hi = ((idx_t)1 << 32);   /* hi^2 > n for any n<2^64 */
    idx_t res = 0;
    while (lo <= hi)
    {
        idx_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * (unsigned __int128)mid;
        if (sq <= n)
        {
            res = mid;
            lo = mid + 1;
        }
        else
            hi = mid - 1;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    idx_t s = isqrt_uint64(n);                /* s = floor(sqrt(n)) */
    idx_t threshold = s * s + s;              /* n <= threshold -> f < 0.5 */

    seq_t two_s = (seq_t)2 * (seq_t)s;        /* 2*s, using seq_t to avoid overflow */

    if (n <= threshold)
        return two_s - 1;    /* a(n) = 2*s - 1 when sqrt(n) < s+0.5 */
    else
        return two_s;        /* a(n) = 2*s    when sqrt(n) >= s+0.5 */
}
