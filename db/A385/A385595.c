/*
 * A385595  The u sequence in quartets (3,u,v,w); i.e., values of u for
 *          solutions to 3*(3+u) = v*(v+w), in positive integers, with u,v>=3
 *          and u>=m, sorted by nondecreasing values of u; see Comments.
 * https://oeis.org/A385595
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the u-values in quartets (3,u,v,w) that satisfy
   3·(3+u) = v·(v+w) with u,v≥3, u≥m=3, sorted by nondecreasing u.
   Each solution (m,u,v,w) contributes one term equal to u. */
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n) {
    idx_t count = 0;
    for (idx_t u = 3;; ++u) {                     /* iterate u in order */
        seq_t N = 3 * ((seq_t)u + 3);             /* RHS of the equation   */
        seq_t sqrtN = floor_sqrt(N);              /* integer floor sqrt    */
        idx_t vmax = (idx_t)sqrtN;                /* safe cast (sqrtN < 2^64) */

        for (idx_t v = 4; v <= vmax; ++v) {       /* v must be ≥4            */
            if (N % (seq_t)v != 0) continue;      /* v divides N?            */
            seq_t w = N / (seq_t)v - (seq_t)v;    /* compute w               */
            if (w > 0) {                          /* w positive?             */
                ++count;
                if (count == n) return (seq_t)u; /* found n-th term         */
            }
        }
    }
}
