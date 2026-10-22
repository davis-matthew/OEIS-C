/*
 * A382359  Number of labeled deterministic finite automata with n states and
 *          two letters.
 * https://oeis.org/A382359
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * 2^n * n^(2*n)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 13,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) /* offset starts at 1, but guard against 0 */
        return (seq_t)0;

    seq_t ns = (seq_t)n;          /* cast index to seq_t */

    /* Compute 2^n (helper pow2 returns a seq_t) */
    seq_t pow2n = pow2(n);

    /* Compute n^(2*n) by repeated multiplication */
    seq_t w = (seq_t)1;
    for (idx_t i = 0; i < (2 * n); ++i)
        w *= ns;

    /* Final result: n * 2^n * n^(2*n) */
    seq_t ans = ns * pow2n;
    ans *= w;
    return ans;
}
