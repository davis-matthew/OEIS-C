/*
 * A253157  Number of (n+1) X (6+1) 0..1 arrays with every 2 X 2 subblock
 *          diagonal minus antidiagonal sum nondecreasing horizontally and
 *          vertically.
 * https://oeis.org/A253157
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 13, u32 n <= 29, u64 n <= 61, u128 n <= 125,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS C implementation for A253157
 * Formula: a(n) = 9*2^(n-1) + 308  for n > 3
 *   a(1) = 325, a(2) = 336, a(3) = 345
 * Recurrence (used by formula): a(n) = 3*a(n-1) - 2*a(n-2)  for n > 5
 *
 * This implementation uses the helper pow2(idx_t exp) to compute 2^exp
 * as a seq_t value.  It works for any seq_t type (8,16,32,64,128 bits
 * or arbitrary-precision integers).
 */

#include <stdbool.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    switch (n) {
        case 1: return (seq_t)325;
        case 2: return (seq_t)336;
        case 3: return (seq_t)345;
        default:
            /* n > 3 */
            seq_t v = pow2(n - 1); /* v = 2^(n-1) */
            v *= (seq_t)9;          /* v = 9 * 2^(n-1) */
            v += (seq_t)308;        /* v = 9*2^(n-1) + 308 */
            return v;
    }
}
