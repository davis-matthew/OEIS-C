/*
 * A360373  Triangular array T read by rows related to the multiplication
 *          table.
 * https://oeis.org/A360373
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = T(n, k) where n = ⌊√(n-1)⌋ + 1 and k = n - (n-1)^2 is the
 *            position within that row
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root: floor(sqrt(x)) for 0 ≤ x < 2^64 */
static inline uint64_t isqrt_uint64(uint64_t x)
{
    uint64_t low  = 0;
    uint64_t high = (x < 0xFFFFFFFFULL) ? x : 0xFFFFFFFFULL; /* √(2^64-1) < 2^32 */
    while (low <= high) {
        uint64_t mid = (low + high) >> 1;
        uint64_t sq  = mid * mid;            /* safe: mid < 2^32 */
        if (sq == x)
            return mid;
        if (sq < x) {
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return high;
}

seq_t a(idx_t n)
{
    /* offset 1: sequence starts at n = 1 */
    if (n == 0) return 0; /* undefined region, return 0 */

    /* find row number */
    uint64_t root = isqrt_uint64(n - 1);   /* floor(sqrt(n-1)) */
    uint64_t row  = root + 1;              /* 1‑based row index */

    /* offset within the row */
    uint64_t prev_sq = root * root;        /* (row-1)^2 */
    uint64_t j = n - prev_sq;              /* 1 ≤ j ≤ 2*row-1 */

    /* compute the factor on the right side of the product */
    uint64_t side = (j <= row) ? j : (2 * row - j);

    return (seq_t)row * (seq_t)side;
}
