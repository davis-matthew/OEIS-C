/*
 * A336753  Largest side of integer-sided triangles whose sides a < b < c are
 *          in arithmetic progression.
 * https://oeis.org/A336753
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest side c of the nth integer‑sided triangle with *
 *            sides a<b<c in arithmetic progression. The triangles are ordered
 *            by * increasing perimeter 3b (i.e., increasing b), and for equal
 *            perimeter * by increasing smallest side a. This routine inverts
 *            that ordering * directly. The function w...
 * Range:     u8 n <= 7140, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   cf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <limits.h>
#include "../utils/cf.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* safe guard, not used by OEIS */

    /* Find the smallest b with cf(b) >= n */
    idx_t low  = 0;
    idx_t high = 1;
    while (cf(high) < n) {
        if (high > UINT64_MAX / 2) { high = UINT64_MAX; break; }
        high <<= 1;
    }
    while (low < high) {
        idx_t mid = (low + high) >> 1;
        if (cf(mid) >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t b = low;                  /* perimeter value b = a + d */

    /* Count of triples before this b */
    idx_t prev_b   = b - 1;
    idx_t cf_prev  = cf(prev_b);

    /* Offset within the current b (0‑based) */
    idx_t offset = n - cf_prev - 1;

    /* Compute sides a and c */
    idx_t a_val = (b >> 1) + 1 + offset;   /* a = floor(b/2)+1+offset   */
    idx_t c_val = b + b - a_val;           /* c = 2b - a                */

    return (seq_t)c_val;
}
