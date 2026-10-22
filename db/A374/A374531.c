/*
 * A374531  a(n) = A374494(A374494(n)).
 * https://oeis.org/A374531
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = A374531(n) = A374494(A374494(n)) Computed by direct use
 *            of the triangle definition: a(n) = (L-1)*(2L-1) + P(L,R) where L
 *            is the row containing n, R = n - (L-1)*(2L-1), and P(L,R) is
 *            defined piecewise below
 * Range:     u8 n <= 154, u16 n <= 9870, u32 n <= 9870, u64 n <= 9870,
 *            u128 n <= 9870, big any n
 * Helpers:   base_row
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/base_row.h"

seq_t a(idx_t n) {
    if (n < 1) return 0;                 /* undefined for n<1, return 0 */

    /* Find the row L such that (L-1)*(2L-1) < n <= L*(2L+1)   */
    idx_t low = 1, high = 1;
    /* grow high until its base is >= n */
    while (base_row(high) < (seq_t)n) high <<= 1;

    /* binary search for minimal row with base >= n */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (base_row(mid) < (seq_t)n)
            low = mid + 1;
        else
            high = mid;
    }

    idx_t L = low - 1;                     /* row containing n */
    seq_t basePrev = base_row(L);           /* terms before row L+1            */
    idx_t basePrevIdx = (idx_t)basePrev;    /* fits, because basePrev < n      */
    idx_t k = n - basePrevIdx;              /* position inside the row (1‑based) */

    seq_t P;
    idx_t twoL_minus1 = 2 * L - 1;

    if (k < twoL_minus1) {
        if (k & 1) { /* k odd, k < 2L-1 */
            P = ((seq_t)(2 * L)) - (seq_t)k - 1;
        } else {    /* k even, k < 2L-1 */
            P = ((seq_t)(4 * L)) - (seq_t)k;
        }
    } else if (k == twoL_minus1) {          /* k == 2L-1 (odd) */
        P = (seq_t)k;
    } else if (k == 2 * L) {                /* k == 2L (even) */
        P = (seq_t)k;
    } else {                                /* k > 2L  */
        if (k & 1) { /* odd */
            P = (seq_t)k;
        } else {    /* even */
            P = (seq_t)(k - (2 * L) - 1);
        }
    }

    return basePrev + P;
}
