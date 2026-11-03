/*
 * A387372  Number of chordless cycles in the complement of the n-Hanoi graph.
 * https://oeis.org/A387372
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = (3^(2*n+2) - 240*3^(n-1) + 183) / 8 for n>=2, a(1)=0
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Validated: partial, pass, 7 terms through n = 7 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/ipow_idx.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n)
{
    if (n == 1) return 0;

    idx_t e_big   = n * 2 + 2;     /* exponent 2*n+2 */
    idx_t e_small = n - 1;         /* exponent n-1  */

    seq_t pow_big   = ipow_idx((seq_t)3, e_big);   /* 3^(2*n+2)   */
    seq_t pow_small = ipow_idx((seq_t)3, e_small); /* 3^(n-1)    */

    seq_t term2 = seq_mul(pow_small, (seq_t)240); /* 240*3^(n-1) */

    seq_t numerator = pow_big - term2 + (seq_t)183;

    return numerator / (seq_t)8;
}
