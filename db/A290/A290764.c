/*
 * A290764  Number of (non-null) connected induced subgraphs in the 2 X n king
 *          graph.
 * https://oeis.org/A290764
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3/4*(3^(n + 1) - 2*n - 3)
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Validated: partial, pass, 27 terms through n = 27 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    seq_t pow3 = ipow((seq_t)3, (seq_t)(n + 1));
    seq_t term = pow3 - 2 * (seq_t)n - 3;
    return 3 * term / 4;
}
