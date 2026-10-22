/*
 * A337191  A version of the Josephus problem: a(n) is the surviving integer
 *          under the skip-eliminate-eliminate version of the elimination
 *          process.
 * https://oeis.org/A337191
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1, a(2) = 1, a(n) = (a(n-2)+3) mod n if that remainder ≠
 *            0, otherwise a(n) = n
 * Range:     u8 n <= 210, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;           /* undefined for n=0; defensive return */

    if (n == 1 || n == 2)
        return (seq_t)1;           /* initial values */

    seq_t prev2 = (seq_t)1;         /* a(1) */
    seq_t prev1 = (seq_t)1;         /* a(2) */

    for (idx_t i = 3; i <= n; ++i) {
        seq_t temp   = prev2 + (seq_t)3;
        seq_t result = temp % (seq_t)i;
        if (result == (seq_t)0)
            result = (seq_t)i;
        prev2 = prev1;
        prev1 = result;
    }

    return prev1;
}
