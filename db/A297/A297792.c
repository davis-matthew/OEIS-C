/*
 * A297792  a(n) = Sum_{d|n} min(d, n/d)^2.
 * https://oeis.org/A297792
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} min(d, n/d)^2
 * Range:     u8 n <= 71, u16 n <= 6719, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Helper: integer square root of an unsigned 64‑bit integer.
   Returns floor(sqrt(n)).  Uses only integer arithmetic. */
static inline idx_t _isqrt_u64(idx_t n)
{
    idx_t low = 0;
    idx_t high = n;
    idx_t res = 0;
    while (low <= high)
    {
        idx_t mid = low + ((high - low) >> 1);
        if (mid == 0)          /* avoid division by zero on n < 1 */
        {
            low = 1;
            continue;
        }
        if (mid <= n / mid)    /* mid*mid <= n  */
        {
            res = mid;
            low = mid + 1;
        }
        else
            high = mid - 1;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* safety for out of range */
    idx_t limit = _isqrt_u64(n);        /* largest d with d*d <= n */
    seq_t sum = 0;
    for (idx_t d = 1; d <= limit; ++d)
    {
        if (n % d == 0)                 /* d is a divisor */
        {
            idx_t other = n / d;        /* paired divisor */
            idx_t m = d <= other ? d : other;   /* min(d, other) */
            seq_t m_sq = (seq_t)m * (seq_t)m;   /* square in seq_t */
            sum += 2 * m_sq;            /* count both d and other */
            if (d == other)             /* perfect square, subtract duplicate */
                sum -= m_sq;
        }
    }
    return sum;
}
