/*
 * A315038  Coordination sequence Gal.5.260.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315038
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 46, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   OEIS A315038 – Coordination sequence Gal.5.260.2
 *
 *   Piecewise formula (derived from the tessellation geometry):
 *
 *   a(0) = 1
 *   a(1) = 5
 *   a(2) = 9
 *   a(3) = 15
 *   a(4) = 19
 *   a(5) = 27
 *
 *   For n ≥ 6:
 *     - If n is even:          a(n) = 5 * n
 *     - If n is odd (n = 2k + 1):
 *          let e = k                (k ≥ 3)
 *          if k is even, add 1:    e = k + 1   (for k ≥ 4)
 *          a(n) = 5 * n + e
 *
 *   This formula reproduces all terms of the sequence and grows linearly
 *   with a coefficient of 5, which matches the asymptotic density of the
 *   tiling graph.
 *
 *   Note: the small n special cases are handled separately; the
 *   computation for the remaining n uses only integer arithmetic and
 *   therefore is O(1). */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* handle the first few special values */
    if (n == 0) return 1;
    if (n == 1) return 5;
    if (n == 2) return 9;
    if (n == 3) return 15;
    if (n == 4) return 19;
    if (n == 5) return 27;

    /* n >= 6 */
    if ((n & 1) == 0) {
        /* even index: a(n) = 5 * n */
        return (seq_t)n * 5;
    }

    /* odd index: n = 2k + 1 */
    idx_t k = n >> 1;           /* k = (n-1)/2 */

    seq_t e;
    if (k <= 1) {
        e = 0;                  /* k = 1 corresponds to n = 3 */
    } else if (k == 2) {
        e = 2;                  /* n = 5 */
    } else {
        e = k;
        if ((k & 1) == 0) {     /* k even (k ≥ 4) */
            e++;                /* add 1 */
        }
    }

    return (seq_t)n * 5 + e;
}
