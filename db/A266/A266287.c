/*
 * A266287  Total number of OFF (white) cells after n iterations of the "Rule
 *          13" elementary cellular automaton starting with a single ON
 *          (black) cell.
 * https://oeis.org/A266287
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n^2+5*n+(-1)^n*(n-1)+1)/4
 * Range:     u8 n <= 9, u16 n <= 275, u32 n <= 999, u64 n <= 999,
 *            u128 n <= 999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    seq_t base = 2*n2 + 5*nn + 1;
    if ((n & 1) == 0) {
        /* even n: add (n-1) */
        base += (nn - 1);
    } else {
        /* odd n: subtract (n-1) */
        base -= (nn - 1);
    }
    return base / 4;
}
