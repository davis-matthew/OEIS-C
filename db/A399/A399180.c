/*
 * A399180  Period 7: repeat 0,1,2,1,2,1,0.
 * https://oeis.org/A399180
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = {0,1,2,1,2,1,0}[n mod 7]
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* 7‑term periodic pattern */
    static const seq_t pat[7] = {0, 1, 2, 1, 2, 1, 0};
    /* idx_t may be 64‑bit unsigned; modulo 7 is safe */
    idx_t r = n % 7;
    /* cast to size_t for array index – safe because r < 7 */
    return pat[(size_t)r];
}
