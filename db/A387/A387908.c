/*
 * A387908  a(n) is the number of distinct integer-sided triangles with
 *          perimeter 6*n and integer area whose sides are in arithmetic
 *          progression.
 * https://oeis.org/A387908
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of distinct integer-sided triangles with perimeter
 *            6*n * and integer area whose sides form an arithmetic
 *            progression. * The sides are (2*n - d, 2*n, 2*n + d) with 1 ≤ d
 *            < n. * A triangle has integer area iff d² + 3*m² = n² for some
 *            integer m ≥ 1. * Hence a(n) counts solutions to d²...
 * Range:     u8 n <= 13, u16 n <= 87, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* zero perimeter yields no triangle */
    if (n == 0)
        return 0;

    seq_t cnt = 0;
    seq_t nn = (seq_t)n * (seq_t)n;         /* n² as seq_t */

    for (idx_t d = 1; d < n; ++d) {
        seq_t dd = (seq_t)d * (seq_t)d;     /* d² */
        seq_t diff = nn - dd;              /* n² – d² > 0 because d < n */

        /* n² – d² must be divisible by 3 */
        if (diff % 3 != 0)
            continue;

        seq_t m2 = diff / 3;               /* m² = (n² – d²)/3 */

        seq_t r = floor_sqrt(m2);          /* integer square root */
        if (r * r == m2)                   /* perfect square? */
            ++cnt;
    }
    return cnt;
}
