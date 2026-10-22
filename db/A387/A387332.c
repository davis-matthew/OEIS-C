/*
 * A387332  Number of binary strings of length n that contain at least one
 *          maximal run of ones of even length.
 * https://oeis.org/A387332
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of binary strings of length n that contain at
 *            least one maximal run of ones of even length. Computation: a(n)
 *            = 2^n - f(n), where f(n) counts strings in which all maximal
 *            runs of ones have odd length. f(n) is obtained by a simple DP
 *            with states: 0 – currently not in a run of ones (l...
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t dp0 = 1;    /* start in state 0 */
    seq_t dp1 = 0;    /* state 1: odd run */
    seq_t dp2 = 0;    /* state 2: even run */

    for (idx_t i = 0; i < n; ++i) {
        seq_t next0 = dp0 + dp1;        /* add 0 from state0/1 */
        seq_t next1 = dp0 + dp2;        /* add 1 from state0/2 */
        seq_t next2 = dp1;              /* add 1 from state1 */
        dp0 = next0;
        dp1 = next1;
        dp2 = next2;
    }

    seq_t f = dp0 + dp1;                 /* all strings with only odd runs */
    seq_t total = pow2(n);               /* 2^n */
    return total - f;
}
