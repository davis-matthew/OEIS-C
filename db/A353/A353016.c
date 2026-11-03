/*
 * A353016  a(n) = Sum_{k=0..floor(n/2)} (n-2*k)^(2*k).
 * https://oeis.org/A353016
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} (n-2*k)^(2*k)
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 18, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Validated: partial, pass, 28 terms through n = 27 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_i32.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t limit = n / 2;         /* floor(n/2) */
    for (idx_t k = 0; k <= limit; ++k)
    {
        seq_t base = (seq_t)(n - 2 * k);   /* base = n-2*k, cast to seq_t */
        int exp   = (int)(2 * k);          /* exponent = 2*k   */
        seq_t term = ipow_i32(base, exp);
        sum += term;
    }
    return sum;
}
