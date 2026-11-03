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
 * Validated: partial, pass, 46 terms through n = 46 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline seq_t
value_for_k(idx_t n, idx_t k)
{
    /* n ≥ 3, k ≥ 3, k ≤ n */
    idx_t a = n / k;          /* base part size */
    idx_t r = n % k;          /* number of parts of size a+1 */
    idx_t count_small = k - r;   /* parts of size a */
    idx_t count_big   = r;       /* parts of size a+1 */

    seq_t S = (seq_t)n;          /* total number of vertices */

    /* part sizes in seq_t */
    seq_t s_a = (seq_t)a;
    seq_t s_b = s_a + 1;

    /* total sum of squares of part sizes */
    seq_t ssq_total = (seq_t)count_small * s_a * s_a
                    + (seq_t)count_big   * s_b * s_b;

    /* contributions for the parts of size a */
    seq_t C_a = (s_a * (s_a - 1)) / 2;            /* C(a,2) */
    seq_t Q_a = ( (S - s_a) * (S - s_a)
                 - (ssq_total - s_a * s_a) ) / 2;
    seq_t part_a = count_small * C_a * Q_a;

    /* contributions for the parts of size a+1 */
    seq_t C_b = (s_b * (s_b - 1)) / 2;            /* C(a+1,2) */
    seq_t Q_b = ( (S - s_b) * (S - s_b)
                 - (ssq_total - s_b * s_b) ) / 2;
    seq_t part_b = count_big * C_b * Q_b;

    return part_a + part_b;
}

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
