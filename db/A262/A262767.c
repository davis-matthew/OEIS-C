/*
 * A262767  Minimum perimeter of a rectangle with area n and integer sides.
 * https://oeis.org/A262767
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2 * (d + n/d) where d is the largest divisor of n not
 *            exceeding ⌊√n⌋. This gives the minimum perimeter of an
 *            integer-sided rectangle with area n
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uint64_t if needed */

static idx_t isqrt_u64(idx_t n)
{
    /* integer square root: floor(sqrt(n)) */
    idx_t low = 0;
    idx_t high = n;
    while (low < high) {
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
    /* n >= 1 is guaranteed by the caller. */
    idx_t root = isqrt_u64(n);              /* ⌊√n⌋ */
    for (idx_t d = root; d >= 1; --d) {
        if (n % d == 0) {
            seq_t e   = (seq_t)(n / d);     /* the paired divisor */
            seq_t sum = (seq_t)d + e;       /* d + e */
            return 2 * sum;                /* perimeter */
        }
    }
    return 0;  /* unreachable – d=1 always divides n */
}
