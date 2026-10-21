/*
 * A284122  Number of binary words w of length n for which s, the longest
 *          proper suffix of w that appears at least twice in w, is of length
 *          1 (i.e., either s = 0 or s = 1).
 * https://oeis.org/A284122
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*F(n-1) + 2*n - 4 for n >= 2; a(1) = 0
 * Range:     u8 n <= 12, u16 n <= 24, u32 n <= 47, u64 n <= 93, u128 n <= 185,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"
seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    seq_t fn = fib(n - 1);           /* F(n-1), n>=2 */
    seq_t res = fn * 2;              /* 2*F(n-1) */
    res += (seq_t)n * 2;             /* +2*n */
    res -= 4;                        /* -4 */
    return res;
}
