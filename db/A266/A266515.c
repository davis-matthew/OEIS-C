/*
 * A266515  Binary representation of the n-th iteration of the "Rule 29"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A266515
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10^n for even n; a(n) = (10*(100^n - 9*10^n) - 1)/9 for
 *            odd n
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 4, u64 n <= 10, u128 n <= 20,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n) {
    if ((n & 1) == 0)          /* even n */
        return pow10(n);
    /* odd n */
    seq_t p2n = pow10(2 * n);   /* 100^n = 10^(2n) */
    seq_t p1n = pow10(n);       /* 10^n    */
    seq_t v = p2n;
    v -= p1n * 9;               /* 100^n - 9*10^n */
    v *= 10;                    /* *10 */
    v -= 1;                     /* -1 */
    v /= 9;                     /* division by 9 */
    return v;
}
