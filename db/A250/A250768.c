/*
 * A250768  Number of (n+1) X (7+1) 0..1 arrays with nondecreasing
 *          x(i,j)-x(i,j-1) in the i direction and nondecreasing absolute
 *          value of x(i,j)-x(i-1,j) in the j direction.
 * https://oeis.org/A250768
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 81*2^(n-1) + 198*n + 240
 * Range:     u8 none, u16 n <= 10, u32 n <= 26, u64 n <= 58, u128 n <= 122,
 *            big any n
 * Validated: standard, pass, 43 terms through n = 208 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/ipow_idx.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n)
{
    /* compute 81*2^(n-1) + 198*n + 240 */
    seq_t pow2   = ipow_idx((seq_t)2, n - 1);
    seq_t result = seq_mul((seq_t)81, pow2);
    result = result + seq_mul((seq_t)198, (seq_t)n);
    result = result + (seq_t)240;
    return result;
}
