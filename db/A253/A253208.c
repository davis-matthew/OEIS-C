/*
 * A253208  a(n) = 4^n + 3.
 * https://oeis.org/A253208
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4^n + 3
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Validated: partial, pass, 25 terms through n = 24 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    /* Compute 4^n using the provided big integer power routine */
    seq_t val = ipow((seq_t)4, (seq_t)n);
    /* Add 3 to obtain a(n) = 4^n + 3 */
    val += (seq_t)3;
    return val;
}
