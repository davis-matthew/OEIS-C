/*
 * A316355  2k-1 appears 2k times after 2k-2 appears once.
 * https://oeis.org/A316355
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) generates a sequence where 2k-2 appears once and 2k-1
 *            appears 2k times, for k = 1,2,3,... a(n) can be computed by
 *            locating the block containing n and selecting the appropriate
 *            value
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Integer square root for uint64_t using binary search.
   Returns floor(sqrt(n)).  This function uses only integer
   arithmetic and works in O(log U) time. */
static inline idx_t isqrt_u64(idx_t n) {
    idx_t low = 0;
    idx_t high = ((idx_t)1 << 32);   /* 2^32 is larger than sqrt(2^64) */
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        __int128 m = (__int128)mid * mid;
        if (m <= (__int128)n) {
            low = mid + 1;
        } else {
            high = mid;
        }
    }
    return low - 1;
}

/* Main sequence function */
seq_t a(idx_t n) {
    /* k is floor(sqrt(n+1)) */
    idx_t k = isqrt_u64(n + 1);

    /* start index of block k: k^2 - 1 (using 128‑bit to avoid overflow) */
    __int128 kk = (__int128)k * k;
    idx_t start = (idx_t)(kk - 1);

    /* position inside the block */
    idx_t pos = n - start;

    if (pos == 0) {
        /* first element of the block: 2k-2 */
        return (seq_t)(2 * k - 2);
    } else {
        /* remaining 2k elements of the block: 2k-1 */
        return (seq_t)(2 * k - 1);
    }
}
