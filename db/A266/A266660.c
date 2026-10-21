/*
 * A266660  Binary representation of the n-th iteration of the "Rule 47"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A266660
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 for n = 0 110 for n = 1 11 for n = 2 11 for even n > 2
 *            (10 * 100^n - 100) / 9 for odd n ≥ 3
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 4, u64 n <= 10, u128 n <= 20,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    /* Special small indices */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)110;
    if (n == 2) return (seq_t)11;

    /* Even indices greater than 2 */
    if ((n & 1) == 0) return (seq_t)11;

    /* Odd indices (n ≥ 3): (10 * 100^n - 100) / 9 */
    seq_t t = pow_seq((seq_t)100, (int)n);   /* 100^n */
    t = (seq_t)10 * t;                      /* 10 * 100^n */
    t -= (seq_t)100;                        /* - 100 */
    t /= (seq_t)9;                          /* / 9 */
    return t;
}
