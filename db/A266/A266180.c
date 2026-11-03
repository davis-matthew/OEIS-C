/*
 * A266180  Decimal representation of the n-th iteration of the "Rule 6"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A266180
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4^(n-1)*(5-(-1)^n)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Validated: standard, pass, 41 terms through n = 969 (2026-11-01)
 * Helpers:   ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent: a(0)=1; a(1)=6; for n>=2: if n is even a(n)=4^n else a(n)=6*4^(n-1) */
#include <stdint.h>
#include "../utils/ipow_u64_idx.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if ((n & 1) == 0)           /* even n */
        return ipow_u64_idx(4, n);
    else                        /* odd n */
        return (seq_t)6 * ipow_u64_idx(4, n - 1);
}
