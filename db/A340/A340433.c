/*
 * A340433  Number of sets in the geometry determined by the Hausdorff metric
 *          at each location between two sets defined by a complete bipartite
 *          graph K(4,n) (with n at least 4) missing three edges, where all
 *          three removed edges are incident to different vertices in the
 *          4-point set but all three removed edges...
 * https://oeis.org/A340433
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 15^(n-1) - 3*7^(n-1) + 3^n - 1
 * Range:     u8 none, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 33,
 *            big any n
 * Validated: standard, pass, 32 terms through n = 738 (2026-11-01)
 * Helpers:   ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_u64_idx.h"

seq_t a(idx_t n)
{
    if (n < 4)
        return 0;                 /* undefined for n < 4, return 0 */

    idx_t exp1 = n - 1;           /* exponent for 15 and 7   */
    idx_t exp2 = n;               /* exponent for 3          */

    /* compute the three powers */
    seq_t t1 = ipow_u64_idx(15ULL, exp1);
    seq_t t2 = ipow_u64_idx(7ULL,  exp1);
    seq_t t3 = ipow_u64_idx(3ULL,  exp2);

    /* apply the formula: t1 - 3*t2 + t3 - 1 */
    seq_t val = t1 - 3 * t2;
    val = val + t3 - 1;

    return val;
}
