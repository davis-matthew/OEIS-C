/*
 * A334603  Period of the fraction 1/11^n for n >= 1.
 * https://oeis.org/A334603
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 2 * 11^(n-1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 20,
 *            big any n
 * Validated: partial, pass, 20 terms through n = 20 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* offset is 1; 0 is unused */
    seq_t exp = (seq_t)(n - 1);
    seq_t pow_val = ipow((seq_t)11, exp);
    return pow_val * (seq_t)2;
}
