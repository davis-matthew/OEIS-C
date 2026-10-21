/*
 * A265381  Decimal representation of the middle column of the "Rule 158"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A265381
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(7*2^(n+2)/15)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 126,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
#if defined(__SIZEOF_INT128__) && (__SIZEOF_LONG_LONG__ == 8)
    /* 64‑bit builds: use 128‑bit arithmetic for n up to 63
       (the largest n that still fits in seq_t). */
    if (sizeof(seq_t) <= 8 && n + 2 < 128) {
        unsigned __int128 num = (unsigned __int128)7 << (n + 2);
        num /= 15;
        return (seq_t)num;
    }
#endif
    /* General case: rely on arbitrary‑precision pow2 when needed.  */
    seq_t val = pow2(n + 2);   /* 2^(n+2) */
    val *= (seq_t)7;           /* multiply by 7 */
    val /= (seq_t)15;          /* integer division -> floor */
    return val;
}
