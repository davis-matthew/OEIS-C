/*
 * A352668  Maximum number of induced copies of the diamond graph K_{1,1,2} in
 *          an n-node graph.
 * https://oeis.org/A352668
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the maximum number of induced copies of the diamond
 *            graph K1,1,2 in an n‑vertex graph. The maximum is attained for a
 *            complete multipartite graph. For a partition with part sizes s1
 *            … sk (k ≥ 3), the number of induced diamonds is Σi C(si,2) ⋅
 *            Σ_{j<ℓ, j≠i, ℓ≠i} sj · sl For a given k the opt...
 * Range:     u8 n <= 11, u16 n <= 41, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   value_for_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/value_for_k.h"

seq_t
a(idx_t n)
{
    /* For n ≤ 3 no diamond can be formed */
    if (n < 4) return 0;

    seq_t best = 0;
    for (idx_t k = 3; k <= n; ++k) {
        seq_t val = value_for_k(n, k);
        if (val > best) best = val;
    }
    return best;
}
