/*
 * A349489  a(n) = Sum_{k=1..n} k * floor(sqrt(2*k-1)).
 * https://oeis.org/A349489
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1..n} k * floor( sqrt(2*k-1) )
 * Range:     u8 n <= 11, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static idx_t isqrt_u64(idx_t n)
{
    /* floor(sqrt(n)) for an unsigned 64‑bit integer, no floating point */
    idx_t low = 0;
    idx_t high = ((idx_t)1 << 32) - 1;   /* sqrt(UINT64_MAX) */
    idx_t ans = 0;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        /* avoid overflow: mid*mid > n  <=>  mid > n/mid  (mid>0) */
        if (mid != 0 && mid > n / mid) {
            high = mid - 1;
        } else {
            ans = mid;
            low = mid + 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        idx_t v = (k << 1) - 1;          /* 2*k - 1 */
        idx_t t = isqrt_u64(v);          /* floor(sqrt(2*k-1)) */
        sum += (seq_t)k * (seq_t)t;      /* accumulate k * t */
    }
    return sum;
}
