/*
 * A343372  Number of sets in the geometry determined by the Hausdorff metric
 *          at each location between two sets defined by a complete bipartite
 *          graph K(3,n) (with n at least 4) missing three edges, where
 *          exactly two removed edges are incident to the same vertex in the
 *          3-point set and exactly two removed edge...
 * https://oeis.org/A343372
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3*7^(n-2) - 4*3^(n-2) + 1
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Validated: partial, pass, 17 terms through n = 20 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    if (n < 4) return 0;            /* offset: values below 4 are undefined */

    seq_t exp = (seq_t)(n - 2);     /* exponent n-2 */

    seq_t p7 = ipow((seq_t)7, exp);   /* 7^(n-2) */
    seq_t p3 = ipow((seq_t)3, exp);   /* 3^(n-2) */

    seq_t res = (seq_t)3 * p7;      /* 3 * 7^(n-2) */
    res -= (seq_t)4 * p3;           /* minus 4 * 3^(n-2) */
    res += (seq_t)1;                /* plus 1 */

    return res;
}
