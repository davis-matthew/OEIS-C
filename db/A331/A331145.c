/*
 * A331145  Triangle read by rows: T(n,k) (n>=k>=1) =
 *          ceiling((n/k)*ceiling(n/k)).
 * https://oeis.org/A331145
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = ceil((n/k) * ceil(n/k)). a(n) is the value in the
 *            triangle read by rows (row index ≥ column index). Indexing
 *            starts at 1. a(1)=1, a(2)=4, a(3)=1, a(4)=9, …
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline seq_t ceil_div(__int128 a, idx_t b) { return (seq_t)((a + b - 1) / b); }

seq_t a(idx_t n) {
    /* Find the row i such that (i-1)i/2 < n ≤ i(i+1)/2 */
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        __int128 tri = (__int128)mid * (mid + 1) / 2;
        if (tri >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t i = low;
    __int128 tri_prev = (__int128)(i - 1) * i / 2;
    idx_t j = (idx_t)(n - tri_prev);          /* column index */
    idx_t r = (i + j - 1) / j;                /* ceil(i/j) */
    __int128 prod = (__int128)i * r;          /* i * ceil(i/j) */
    __int128 ans128 = (prod + j - 1) / j;     /* ceil((i/j) * ceil(i/j)) */
    return (seq_t)ans128;
}
