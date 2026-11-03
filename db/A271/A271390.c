/*
 * A271390  a(n) = (2*n + 1)^(2*floor((n-1)/2) + 1).
 * https://oeis.org/A271390
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n + 1)^(2*floor((n-1)/2) + 1) (offset 0)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 14, u128 n <= 23,
 *            big any n
 * Validated: standard, pass, 35 terms through n = 73 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    /* Special case for a(0) – the exponent would be negative, but 1^k = 1 */
    if (n == 0)
        return 1;

    seq_t base = ((seq_t)2 * n) + 1;
    /* For even n, exponent = n-1; for odd n, exponent = n */
    seq_t exp  = (n & 1) ? (seq_t)n : (seq_t)(n - 1);

    return ipow(base, exp);
}
