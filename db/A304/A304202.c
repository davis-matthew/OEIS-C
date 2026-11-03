/*
 * A304202  a(n) = (2*n-3)*4^(n-1) - 2*binomial(2*n, n-1).
 * https://oeis.org/A304202
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n-3)*4^(n-1) - 2*binomial(2*n, n-1)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Validated: partial, pass, 23 terms through n = 25 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"

static inline seq_t pow4(idx_t exp) {
    seq_t result = 1;
    seq_t base   = 4;
    idx_t e = exp;
    while (e) {
        if (e & 1) result *= base;
        base *= base;
        e >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* OEIS offset is 3, but the implementation works for all n >= 3. */
    if (n < 3) return 0;            /* undefined for n < 3 */

    /* 4^(n-1) */
    seq_t pow4_val = pow4(n - 1);

    /* (2*n - 3) * 4^(n-1) */
    seq_t term1 = (seq_t)(2 * n - 3) * pow4_val;

    /* 2 * binomial(2*n, n-1) */
    seq_t bin = binomial(2 * n, n - 1);
    seq_t term2 = 2 * bin;

    /* Result */
    seq_t res = term1 - term2;
    return res;
}
