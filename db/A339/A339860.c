/*
 * A339860  Perimeter of primitive integer-sided triangles whose sides a < b <
 *          c form a geometric progression.
 * https://oeis.org/A339860
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the perimeter of the nth primitive triangle with integer
 *            sides in geometric progression. The sides are (r², r·s, s²)
 *            where 1 < r < s, gcd(r,s)=1, and the triangle inequality r² +
 *            r·s > s² holds. The triples are ordered by increasing r, then
 *            increasing s
 * Range:     u8 n <= 12, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    for (idx_t r = 2; ; ++r) {
        seq_t rt = (seq_t)r;
        for (idx_t s = r + 1; ; ++s) {
            seq_t st = (seq_t)s;
            seq_t left  = rt * rt + rt * st; /* r² + r·s */
            seq_t right = st * st;           /* s²   */
            if (left <= right)
                break;                      /* triangle inequality fails for larger s */
            if (gcd(rt, st) == (seq_t)1) { /* primitive condition */
                ++count;
                if (count == n)
                    return rt * rt + rt * st + st * st; /* r² + r·s + s² */
            }
        }
    }
}
