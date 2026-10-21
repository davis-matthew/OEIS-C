/*
 * A266257  Total number of ON (black) cells after n iterations of the "Rule
 *          11" elementary cellular automaton starting with a single ON
 *          (black) cell.
 * https://oeis.org/A266257
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((n+1)^2 - (-1)^n*(n-1))/2
 * Range:     u8 n <= 14, u16 n <= 258, u32 n <= 999, u64 n <= 999,
 *            u128 n <= 999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    seq_t nn = (seq_t)n;
    seq_t t1 = (nn + 1) * (nn + 1);
    seq_t t2 = nn - 1;
    if (n & 1ULL) {    /* n is odd: add (n-1) */
        return (t1 + t2) / 2;
    } else {           /* n is even: subtract (n-1) */
        return (t1 - t2) / 2;
    }
}
