/*
 * A257288  a(n) = 7*8^n-6*7^n.
 * https://oeis.org/A257288
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*8^n-6*7^n
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Validated: partial, pass, 21 terms through n = 20 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    seq_t pow8 = ipow((seq_t)8, (seq_t)n);
    seq_t pow7 = ipow((seq_t)7, (seq_t)n);
    return ((seq_t)7 * pow8) - ((seq_t)6 * pow7);
}
