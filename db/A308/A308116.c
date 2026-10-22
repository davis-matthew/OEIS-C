/*
 * A308116  Sum of the perimeters of the integer-sided triangles with
 *          perimeter n whose side lengths are squarefree.
 * https://oeis.org/A308116
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * A308061(n) where A308061(n) counts integer‑sided
 *            triangles with perimeter n whose side lengths are all squarefree
 * Range:     u8 n <= 30, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For each n, we enumerate all integer triples (a,b,c) with
   a ≤ b ≤ c, a+b>c, a+b+c=n, test squarefreeness, and sum n for
   each valid triangle.  */

#include <stdbool.h>
#include "../utils/is_squarefree.h"

static inline seq_t count_squarefree_triangles(idx_t n)
{
    seq_t cnt = 0;
    for (idx_t a = 1; a <= n / 3; ++a) {
        idx_t max_b = (n - a) / 2;
        for (idx_t b = a; b <= max_b; ++b) {
            idx_t c = n - a - b;
            if (c < b) break;                /* safety, should not happen */
            if (2 * (a + b) <= n) continue;   /* violates triangle inequality */
            if (is_squarefree(a) && is_squarefree(b) && is_squarefree(c))
                ++cnt;
        }
    }
    return cnt;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset starts at 1, but guard anyway */
    seq_t cnt   = count_squarefree_triangles(n);
    seq_t result = (seq_t)n * cnt;   /* total perimeter sum */
    return result;
}
