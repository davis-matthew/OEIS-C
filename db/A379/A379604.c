/*
 * A379604  a(n) is the maximum number of items in a bucket for the bucket
 *          sort algorithm with input {0, 1, ..., n-1} and B = floor(sqrt(n))
 *          buckets.
 * https://oeis.org/A379604
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil(n / floor(sqrt(n)))
 * Range:     u8 n <= 408, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer floor square root for 64‑bit unsigned integers.
   Uses binary search; all arithmetic is safe from overflow. */
static idx_t _isqrt_u64(idx_t n)
{
    idx_t low = 0, high = n;
    while (low < high)
    {
        idx_t mid = (low + high + 1) >> 1;
        if (mid <= n / mid)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                       /* offset is 1 – n=0 not used */
    idx_t B = _isqrt_u64(n);            /* floor(sqrt(n))  */
    seq_t numerator = (seq_t)n + (seq_t)(B - 1);  /* ceil division: (n+B-1)/B */
    return numerator / (seq_t)B;
}
