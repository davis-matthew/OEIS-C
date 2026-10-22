/*
 * A320278  a(n) is the number of positive integers 0 < i < n such that i + n
 *          is a square.
 * https://oeis.org/A320278
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(sqrt(max(0,2*n-1))) - floor(sqrt(n))
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* implementation uses its own integer-square-root routine (O(log n)) */

#include <stdbool.h>

/* local helper: floor of the integer square root of a 64‑bit unsigned integer */
static inline idx_t _isqrt_u64(idx_t n)
{
    /* Handle trivial cases quickly */
    if (n <= 1) return n;

    /* Binary search bounded by n (or (1ULL<<32) which is sufficient for uint64) */
    idx_t low = 0, high = 1ULL << 32, ans = 0;
    while (low <= high) {
        idx_t mid = (low + high) >> 1;
        /* avoid overflow: mid <= n/mid  is equivalent to mid*mid <= n */
        if (mid <= n / mid) {
            ans = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;   /* prevent infinite loop when mid becomes 0 */
            high = mid - 1;
        }
    }
    return ans;
}

/* Compute the n-th term of OEIS sequence A320278 */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    /* 2*n-1 fits in idx_t for all n we care about */
    idx_t two_n_minus_one = (n << 1) - 1;

    idx_t sqrt1 = _isqrt_u64(two_n_minus_one);
    idx_t sqrt2 = _isqrt_u64(n);

    return (seq_t)(sqrt1 - sqrt2);
}
