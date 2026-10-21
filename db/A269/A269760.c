/*
 * A269760  Number of n X 1 0..5 arrays with some element plus some
 *          horizontally or vertically adjacent neighbor totalling five
 *          exactly once.
 * https://oeis.org/A269760
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 0, a(2) = 6, a(n) = 10*a(n-1) - 25*a(n-2) for n >= 3
 *            (equivalently a(n) = 6*5^(n-2)*(n-1) for n >= 2)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 26, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 1) return 0;
    if (n == 2) return 6;

    seq_t prev2 = 0;   /* a(1) */
    seq_t prev1 = 6;   /* a(2) */
    seq_t cur;

    for (idx_t i = 3; i <= n; ++i) {
        /* compute a(i) = 10*prev1 - 25*prev2 */
        cur = 10 * prev1 - 25 * prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
