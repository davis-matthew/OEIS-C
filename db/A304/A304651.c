/*
 * A304651  Number of coprime pairs (x,y) with x^2 + y^2 <= n.
 * https://oeis.org/A304651
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of coprime integer pairs (x,y) with x² + y² ≤ n,
 *            where (0,0) is excluded. The implementation enumerates unordered
 *            pairs with x ≥ y ≥ 0, counts each with the appropriate symmetry
 *            factor (4 for x = 0 or y = 0 or x = y, 8 otherwise), and sums.
 *            It uses a small binary‑search integer squa...
 * Range:     u8 n <= 129, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

/* Integer square root: floor(sqrt(n)) using binary search; 
   handles n = 0 correctly. */
static inline idx_t isqrt(idx_t n)
{
    if (n < 2) return n;          /* 0 or 1 */
    idx_t low  = 1;
    idx_t high = n >> 1;          /* sqrt(n) ≤ n/2 for n≥4 */
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        if (mid > n / mid) {      /* mid² > n */
            high = mid - 1;
        } else {                  /* mid² ≤ n */
            if (mid + 1 > n / (mid + 1))
                return mid;
            low = mid + 1;
        }
    }
    return low;                    /* never reached */
}

/* Main sequence function */
seq_t a(idx_t n)
{
    seq_t count = 0;
    idx_t r = isqrt(n);            /* maximum |x| or |y| */
    for (idx_t x = 0; x <= r; ++x) {
        idx_t rem   = n - x * x;
        idx_t ymax  = isqrt(rem);
        idx_t limit = (ymax < x) ? ymax : x;   /* y ≤ x, y ≥ 0 */
        for (idx_t y = 0; y <= limit; ++y) {
            if (x == 0 && y == 0) continue;   /* skip (0,0) */
            if (gcd((seq_t)x, (seq_t)y) != 1) continue;
            if (x == 0 || y == 0 || x == y)
                count += (seq_t)4;            /* axis or equal */
            else
                count += (seq_t)8;            /* generic case */
        }
    }
    return count;
}
