/*
 * A370706  Triangle read by rows: T(n, k) = binomial(n, k) * Pochhammer(n,
 *          k).
 * https://oeis.org/A370706
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   * a(n) = T(r, c) where (r, c) satisfy * r*(r+1)/2 <= n <
 *            (r+1)*(r+2)/2 and c = n - r*(r+1)/2 * T(r, c) = binomial(r, c) *
 *            prod_{i=0}^{c-1} (r + i) * (binomial is from
 *            ../utils/binomial.h)
 * Range:     u8 n <= 12, u16 n <= 25, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include <stdint.h>
 
/* Find the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
static idx_t find_row(idx_t n)
{
    idx_t low = 0;
    idx_t high = 1;
    /* increase high until the triangular number exceeds n */
    while (((__int128)high * (high + 1)) / 2 <= n) {
        if (high == 0) {          /* overflow guard */
            high = n + 1;
            break;
        }
        high <<= 1;
    }
    /* binary search between low and high-1 */
    while (low < high) {
        idx_t mid = low + (high - low + 1) / 2;
        if (((__int128)mid * (mid + 1)) / 2 <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}
 
seq_t a(idx_t n)
{
    idx_t r = find_row(n);
    idx_t c = n - r * (r + 1) / 2;
 
    /* binomial(r, c) */
    seq_t val = binomial(r, c);
 
    /* multiply by the rising factorial factor */
    for (idx_t i = 0; i < c; ++i)
        val = val * (seq_t)(r + i);
 
    return val;
}
