/*
 * A399046  Total number of connected components in all induced subgraphs of
 *          the n X 3 rook graph.
 * https://oeis.org/A399046
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8^n + 3*5^n - 4*4^n
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Validated: standard, pass, 37 terms through n = 481 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/ipow_idx.h"

seq_t a(idx_t n)
{
    seq_t p8 = ipow_idx((seq_t)8, n);
    seq_t p5 = ipow_idx((seq_t)5, n);
    seq_t p4 = ipow_idx((seq_t)4, n);
    seq_t result = p8 + p5 * (seq_t)3;
    result -= p4 * (seq_t)4;
    return result;
}
