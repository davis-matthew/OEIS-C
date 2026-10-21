/*
 * A280316  Sum of squares of parts of the partitions of 2n into two
 *          squarefree parts.
 * https://oeis.org/A280316
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..n} (i^2 + (2n-i)^2) * mu(i)^2 * mu(2n-i)^2 Here
 *            we use mu(i)^2 = 1 if i is squarefree, else 0. So the sum runs
 *            only over i and 2n-i that are squarefree. The function
 *            is_squarefree supplied by the utils library is used for the
 *            square‑free check
 * Range:     u8 n <= 5, u16 n <= 35, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_squarefree.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset starts at 1, but guard zero */
    idx_t two_n = n << 1;           /* 2*n, remains within idx_t range */
    seq_t sum = 0;
    for (idx_t i = 1; i <= n; ++i) {
        if (is_squarefree((seq_t)i) && is_squarefree((seq_t)(two_n - i))) {
            seq_t ii = (seq_t)i;
            seq_t jj = (seq_t)(two_n - i);
            sum += ii * ii + jj * jj;
        }
    }
    return sum;
}
