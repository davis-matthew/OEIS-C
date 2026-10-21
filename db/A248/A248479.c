/*
 * A248479  a(1) = 1, a(2) = 3, and from then on alternatively subtract and
 *          multiply two previous terms.
 * https://oeis.org/A248479
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1, a(2) = 3; for n>=3: if n is odd: a(n) = a(n-1) -
 *            a(n-2); if n is even: a(n) = a(n-1) * a(n-2);
 * Range:     u8 n <= 7, u16 n <= 9, u32 n <= 11, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined, but harmless */

    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)3;

    seq_t a_prev2 = (seq_t)1;   /* a(1) */
    seq_t a_prev1 = (seq_t)3;   /* a(2) */
    seq_t a_cur = 0;

    for (idx_t i = 3; i <= n; ++i) {
        if (i & 1U)               /* odd index */
            a_cur = a_prev1 - a_prev2;
        else                      /* even index */
            a_cur = a_prev1 * a_prev2;

        a_prev2 = a_prev1;
        a_prev1 = a_cur;
    }

    return a_cur;
}
