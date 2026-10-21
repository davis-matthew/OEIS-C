/*
 * A265721  Decimal representation of the n-th iteration of the "Rule 1"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A265721
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n for n even a(n) = 2*4^n - 7*2^(n-1) - 1 for n odd
 *            (derived from the given closed forms)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* even n: a(n) = 2^n */
    if ((n & 1U) == 0)
        return pow2(n);

    /* odd n: a(n) = 2*4^n - 7*2^(n-1) - 1
       2*4^n = 2^(2n+1)                         */
    seq_t term = pow2(2 * n + 1);   /* 2^(2n+1) */
    term -= 7 * pow2(n - 1);       /* 7*2^(n-1)   */
    term -= 1;                     /* subtract 1   */
    return term;
}
