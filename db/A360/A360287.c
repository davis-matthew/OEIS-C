/*
 * A360287  a(n) is the concatenation of the positions of 1-bits in the binary
 *          expansion of the Gray code for n, when 1 is the rightmost
 *          position; a(0) = 0.
 * https://oeis.org/A360287
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look, base
 * Formula:   a(n) = concatenation of positions of 1‑bits in the binary Gray
 *            code of n (positions counted from 1 at the rightmost bit). a(0)
 *            = 0
 * Range:     u8 n <= 9, u16 n <= 41, u32 n <= 681, u64 n <= 9714,
 *            u128 n <= 9714, big any n
 * Helpers:   digits, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/digits.h"   /* digits(idx_t) returns decimal digit count */
#include "../utils/pow10.h"    /* pow10(idx_t) returns 10^e as seq_t */

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;
    }

    idx_t g = n ^ (n >> 1);  /* Gray code of n */
    seq_t result = (seq_t)0;

    for (idx_t pos = 1; g; ++pos, g >>= 1) {
        if (g & 1) {
            /* multiply current result by 10^digits(pos) and add pos */
            seq_t mult = pow10((idx_t)digits(pos));
            result = result * mult + (seq_t)pos;
        }
    }

    return result;
}
