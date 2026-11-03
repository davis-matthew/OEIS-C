/*
 * A289719  a(n) = (n/2)*binomial(2*n, n) + 1.
 * https://oeis.org/A289719
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n/2)*binomial(2*n, n) + 1
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Validated: standard, pass, 42 terms through n = 464 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t c = binomial(2*n, n);
    seq_t res = ((c / 2) * (seq_t)n) + 1;
    return res;
}
