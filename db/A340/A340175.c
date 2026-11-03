/*
 * A340175  Number of sets in the geometry determined by the Hausdorff metric
 *          at each location between two sets defined by a complete bipartite
 *          graph K(6,n) (with n at least 3) missing two edges, where the two
 *          removed edges are not incident to the same vertex in the 6-point
 *          set but are incident to the same v...
 * https://oeis.org/A340175
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 15*63^(n-1) - 58*31^(n-1) + 89*15^(n-1) - 68*7^(n-1) +
 *            26*3^(n-1) - 4
 * Range:     u8 none, u16 n <= 3, u32 n <= 5, u64 n <= 11, u128 n <= 21,
 *            big any n
 * Validated: standard, pass, 29 terms through n = 423 (2026-11-01)
 * Helpers:   ipow_u64_idx, ipow_u64_idx_to_i128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_u64_idx.h"
#include "../utils/ipow_u64_idx_to_i128.h"

seq_t a(idx_t n)
{
    if (n < 3) return 0;    /* not defined below offset */

    /* Use 128‑bit arithmetic for 64‑bit seq_t and n <= 10 (the largest n that
       fits in 64‑bit without overflow in the intermediate terms).  For larger
       n we fall back to the arbitrary‑precision pow_uint routine. */
    if (sizeof(seq_t) <= 8 && n > 10) {
        __int128 p63 = ipow_u64_idx_to_i128(63, n-1);
        __int128 p31 = ipow_u64_idx_to_i128(31, n-1);
        __int128 p15 = ipow_u64_idx_to_i128(15, n-1);
        __int128 p7  = ipow_u64_idx_to_i128(7,  n-1);
        __int128 p3  = ipow_u64_idx_to_i128(3,  n-1);

        __int128 res = 15 * p63 - 58 * p31 + 89 * p15
                     - 68 * p7  + 26 * p3  - 4;
        return (seq_t)(uint64_t)res;
    }

    /* Default path uses the big‑integer utility pow_uint. */
    seq_t p63 = ipow_u64_idx(63, n-1);
    seq_t p31 = ipow_u64_idx(31, n-1);
    seq_t p15 = ipow_u64_idx(15, n-1);
    seq_t p7  = ipow_u64_idx(7,  n-1);
    seq_t p3  = ipow_u64_idx(3,  n-1);

    seq_t res = 15 * p63 - 58 * p31 + 89 * p15
              - 68 * p7  + 26 * p3  - 4;
    return res;
}
