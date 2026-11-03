/*
 * A255043  a(n) = (5*9^n - 1)/2.
 * https://oeis.org/A255043
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5*9^n - 1)/2
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Validated: standard, pass, 36 terms through n = 918 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    /* Compute 9^n */
    seq_t pow9 = ipow((seq_t)9, (seq_t)n);

    /* Multiply by 5, subtract 1, then divide by 2 */
    seq_t tmp  = pow9 * (seq_t)5;
    tmp -= (seq_t)1;
    return tmp / (seq_t)2;
}
