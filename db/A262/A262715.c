/*
 * A262715  a(n) = 29^(2*n+1).
 * https://oeis.org/A262715
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 29^(2*n+1)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 2, u64 n <= 6, u128 n <= 12,
 *            big any n
 * Validated: standard, pass, 28 terms through n = 236 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    seq_t base = (seq_t)29;
    seq_t exp  = (seq_t)(2 * n + 1);
    return ipow(base, exp);
}
