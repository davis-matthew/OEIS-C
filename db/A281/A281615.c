/*
 * A281615  Expansion of Sum_{i>=1} x^(i*(i+1)/2)/(1 - x^(i*(i+1)/2)) /
 *          Product_{j>=1} (1 - x^(j*(j+1)/2)).
 * https://oeis.org/A281615
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = total number of parts in all partitions of n into
 *            triangular numbers. The generating function is \sum_{t
 *            triangular} \frac{x^t}{1-x^t} / \prod_{t triangular} (1-x^t)
 * Range:     u8 n <= 20, u16 n <= 78, u32 n <= 377, u64 n <= 2134,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation computes the sequence by dynamic programming.
   For each triangular number t ≤ n let
       p[s]  = number of partitions of s into triangular numbers,
       f[s]  = total number of parts in all such partitions.
   When processing a fixed t we update for all s ≥ t:
       add = p[s-t];
       p[s] += add;                     // new partitions with part t
       f[s] += f[s-t] + add;            // add one part per new partition
   The arrays are rebuilt from scratch for each call; n ≤ 10000
   keeps the computation well under the 2 s limit. */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    /* Allocate arrays of size n+1.  Index 0 is used as the empty partition. */
    seq_t *p = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    seq_t *f = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    if (!p || !f) return 0;          /* memory failure – return 0 */

    p[0] = 1;                         /* there is one way to partition 0  */

    /* Precompute all triangular numbers up to n. */
    for (idx_t i = 1; ; ++i) {
        seq_t t = (seq_t)i * (i + 1) / 2;
        if (t > n) break;
        idx_t t_idx = (idx_t)t;       /* safe cast: t ≤ 5·10⁷ for n ≤ 10⁴ */

        for (idx_t s = t_idx; s <= n; ++s) {
            seq_t add = p[s - t_idx];
            p[s] += add;
            f[s] += f[s - t_idx] + add;
        }
    }

    seq_t ans = f[n];
    free(p);
    free(f);
    return ans;
}
