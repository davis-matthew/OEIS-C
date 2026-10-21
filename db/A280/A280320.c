/*
 * A280320  Sum of the squares of the smaller parts of the partitions of 2n
 *          into two squarefree parts.
 * https://oeis.org/A280320
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{i=1..n} i^2 * μ(i)^2 * μ(2n-i)^2 (where μ(i)^2 = 1 if
 *            i is squarefree, 0 otherwise)
 * Range:     u8 n <= 11, u16 n <= 71, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_squarefree.h"  /* for bool is_squarefree(seq_t) */

seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* behavior below offset is irrelevant */

    seq_t sum = 0;
    idx_t two_n = n << 1;         /* 2*n */

    for (idx_t i = 1; i <= n; ++i) {
        if (is_squarefree((seq_t)i) && is_squarefree((seq_t)(two_n - i))) {
            seq_t ii = (seq_t)i;
            sum += ii * ii;       /* i^2 */
        }
    }
    return sum;
}
