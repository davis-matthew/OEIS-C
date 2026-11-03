/*
 * A259926  a(n) = n^(2*n) - n^(2*n - 1).
 * https://oeis.org/A259926
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^(2*n) - n^(2*n-1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Validated: partial, pass, 15 terms through n = 15 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_idx.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* indices below 1 are undefined by OEIS offset */

    /* Convert index to seq_t for arithmetic */
    seq_t base = (seq_t)n;

    /* Compute exponent 2*n-1 using unsigned arithmetic */
    idx_t exp = n;
    exp += n;          /* 2*n */
    exp -= 1;          /* 2*n-1 */

    /* n^(2*n-1) */
    seq_t pow_minus_one = ipow_idx(base, exp);

    /* n^(2*n) = n^(2*n-1) * n */
    seq_t pow = pow_minus_one * base;

    /* a(n) = n^(2*n) - n^(2*n-1) */
    return pow - pow_minus_one;
}
