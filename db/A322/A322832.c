/*
 * A322832  Values x + y, where the ordered pairs (x,y) are sorted first by
 *          maximal coordinate and then lexicographically.
 * https://oeis.org/A322832
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, look
 * Formula:   For n = m^2 + j with 0 ≤ j ≤ 2m, a(n) = m + j if j < m a(n) = j
 *            otherwise
 * Range:     u8 n <= 8426, u16 n <= 8426, u32 n <= 8426, u64 n <= 8426,
 *            u128 n <= 8426, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static idx_t uint64_floor_sqrt(idx_t n)
{
    idx_t lo = 0, hi = n;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        if (mid == 0) {          /* 0*0 <= n for all n */
            lo = 1;              /* skip the trivial case */
            continue;
        }
        if (mid > n / mid) {     /* mid*mid > n */
            hi = mid - 1;
        } else {
            /* mid*mid <= n; check if (mid+1)^2 would exceed n */
            if ((mid + 1) > n / (mid + 1))
                return mid;
            lo = mid + 1;
        }
    }
    return 0; /* should never be reached for n >= 0 */
}

seq_t a(idx_t n)
{
    idx_t m = uint64_floor_sqrt(n);
    idx_t j = n - m * m;
    if (j < m)
        return (seq_t)m + (seq_t)j;
    else
        return (seq_t)j;
}
