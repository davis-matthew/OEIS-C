/*
 * A343444  Smallest nonnegative integer such that altering at most one of its
 *          digits cannot result in a previous term.
 * https://oeis.org/A343444
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   digit_xor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence defined by scanning integers with decimal digit XOR zero */
#include <stddef.h>
#include "../utils/digit_xor.h"

seq_t a(idx_t n)
{
    idx_t cnt = 0;
    seq_t cur = 0;
    for (;; cur++) {
        if (digit_xor(cur) == 0) {
            cnt++;
            if (cnt == n) return cur;
        }
    }
}
