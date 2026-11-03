/*
 * A261186  a(n) = binomial(3*n-2,n+1).
 * https://oeis.org/A261186
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(3*n-2, n+1)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 12, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Validated: partial, pass, 21 terms through n = 22 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n < 2) return 0;
    return binomial(3*n - 2, n + 1);
}
