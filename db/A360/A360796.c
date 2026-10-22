/*
 * A360796  a(n) > n is the smallest integer such that there exist integers n
 *          < c <= d < a(n) satisfying n^2 + a(n)^2 = c^2 + d^2.
 * https://oeis.org/A360796
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the smallest integer a > n such that there exist *
 *            integers n < c ≤ d < a with n^2 + a^2 = c^2 + d^2. * The
 *            algorithm brute‑forces a from n+1 up to 2*n+5 (proved bound), *
 *            and for each a checks all c in (n, a) to see if the remaining *
 *            term is a perfect square d^2 with n < c ≤ d < a. The...
 * Range:     u8 n <= 1, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n == 0)         /* sequence is defined for n ≥ 1 */
        return 0;

    /* Pre‑compute n^2 once */
    seq_t n_sq = (seq_t)n * (seq_t)n;

    /* Upper bound: a ≤ 2*n + 5 (identity holds for all n) */
    idx_t a_limit = n + n + 5;          /* 2*n + 5 */

    for (idx_t a = n + 1; a <= a_limit; ++a) {
        seq_t A   = (seq_t)a;
        seq_t SA  = n_sq + A * A;       /* S = n^2 + a^2 */

        /* Start c at n+1; pre‑compute its square */
        seq_t c      = (seq_t)(n + 1);
        seq_t c_sq   = (seq_t)(n + 1) * (seq_t)(n + 1);

        /* Iterate c up to a-1 (d < a) */
        for (idx_t c_idx = n + 1; c_idx <= a - 1; ++c_idx) {
            seq_t rem = SA - c_sq;        /* remaining to reach S */

            /* If remaining < c^2 then d < c → impossible */
            if (rem < c_sq)
                break;

            seq_t d = floor_sqrt(rem);
            if (d >= c && d < A && d * d == rem) {
                /* Found a valid representation */
                return A;
            }

            /* Prepare next c^2: (c+1)^2 = c^2 + 2*c + 1 */
            c_sq += 2 * (seq_t)c_idx + 1;
        }
    }

    /* The bound guarantees a solution, so this line should never be reached */
    return 0;
}
