/*
 * A296665  Row sums of A296666.
 * https://oeis.org/A296665
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 * (n + 1) * binomial(2*n, n) - 4^n
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Validated: partial, pass, 25 terms through n = 24 (2026-11-01)
 * Helpers:   binomial, ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/ipow_u64_idx.h"

seq_t a(idx_t n)
{
    /* binomial(2n, n) */
    idx_t two_n = n << 1;          /* 2*n, safe for n up to 2^63-1 */
    seq_t bin = binomial(two_n, n);

    /* factor = 2 * (n + 1) as a seq_t */
    seq_t factor = (seq_t)n;
    factor += 1;
    factor *= 2;

    /* term = factor * bin */
    seq_t term = factor * bin;

    /* subtract 4^n */
    seq_t pow4 = ipow_u64_idx(4, n);
    term -= pow4;

    return term;
}
