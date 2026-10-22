/*
 * A329779  Numbers that are either odd or are congruent to {+-4, +-6, +-8,
 *          +-10} (mod 32).
 * https://oeis.org/A329779
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 32*floor((n-1)/24) + A[ (n-1)%24 ] , where A =
 *            [1,3,4,5,6,7,8,9,10,11,13,15,17,19,21,22,23,24,25,26,27,28,29,31]
 *            (OEIS A329779)
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const seq_t allowed[24] = {
    (seq_t)1,  (seq_t)3,  (seq_t)4,  (seq_t)5,
    (seq_t)6,  (seq_t)7,  (seq_t)8,  (seq_t)9,
    (seq_t)10, (seq_t)11, (seq_t)13, (seq_t)15,
    (seq_t)17, (seq_t)19, (seq_t)21, (seq_t)22,
    (seq_t)23, (seq_t)24, (seq_t)25, (seq_t)26,
    (seq_t)27, (seq_t)28, (seq_t)29, (seq_t)31
};

seq_t a(idx_t n) {
    /* n is 1-indexed; n >= 1 */
    idx_t idx = n - 1;           /* 0-indexed position */
    idx_t cycle = idx / 24;      /* number of full 32-blocks preceding the desired block */
    seq_t base  = (seq_t)cycle * (seq_t)32;   /* base offset of the block */
    idx_t rem   = idx % 24;       /* position within the block */
    return base + allowed[rem];
}
