/*
 * A280510  Index sequence of the Thue-Morse sequence (A010060) as a
 *          block-fractal sequence.
 * https://oeis.org/A280510
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Index sequence of the Thue-Morse sequence (A010060) as a block‑fractal sequence.
   Formula:
     a(1) = 3;
     a(n) = 3 * 2^⌊log₂(n−1)⌋  for n ≥ 2
   (equivalently, a(n) = 3 * A053644(n−1), where A053644(k) is the largest power
   of 2 ≤ k.) */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow2.h"

static idx_t floor_log2_idx(idx_t v)
{
    idx_t exp = 0;
    while (v >> 1) {
        v >>= 1;
        exp++;
    }
    return exp;
}

seq_t a(idx_t n)
{
    if (n <= 1) {
        /* n==1 gives the first term 3;  n==0 is outside the index range but
           returned as 3 for safety. */
        return (seq_t)3;
    }
    idx_t v   = n - 1;           /* argument for floor_log2 */
    idx_t exp = floor_log2_idx(v);/* ⌊log₂(v)⌋ */
    seq_t pow = pow2(exp);       /* 2^exp */
    return pow * (seq_t)3;       /* 3 * 2^exp */
}
