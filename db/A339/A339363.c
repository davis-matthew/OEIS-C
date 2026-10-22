/*
 * A339363  a(n) = Sum_{k=1..floor(sqrt(n))} floor(sqrt(n-k)).
 * https://oeis.org/A339363
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..floor(sqrt(n))} floor(sqrt(n-k))
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute integer square root of an unsigned 64‑bit integer (floor(sqrt)) */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t low = 0;
    idx_t high = n;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        if (mid == 0) {
            low = 1;
            continue;
        }
        /* compare mid*mid <= n without overflow: mid <= n/mid */
        if (mid <= n / mid)
            low = mid + 1;      /* mid is a candidate, look for a larger one */
        else
            high = mid - 1;     /* mid^2 > n, look lower */
    }
    return high; /* highest mid such that mid*mid <= n */
}

seq_t a(idx_t n)
{
    /* Assumes n >= 1 (offset is 1) */
    idx_t m = isqrt_u64(n);   /* floor(sqrt(n)) */
    seq_t sum = 0;
    for (idx_t k = 1; k <= m; ++k) {
        idx_t nk = n - k;             /* n - k >= 0 */
        idx_t t  = isqrt_u64(nk);      /* floor(sqrt(n-k)) */
        sum += (seq_t)t;
    }
    return sum;
}
