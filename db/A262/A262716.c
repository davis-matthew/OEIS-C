/*
 * A262716  a(n) = 31^(2*n+1).
 * https://oeis.org/A262716
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 31^(2*n+1)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 2, u64 n <= 5, u128 n <= 12,
 *            big any n
 * Validated: standard, pass, 28 terms through n = 247 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    seq_t base = 31;
    seq_t exp  = (seq_t)n * 2;
    exp += 1;              /* exp = 2*n + 1 */
    return ipow(base, exp);
}
