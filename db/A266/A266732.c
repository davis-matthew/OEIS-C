/*
 * A266732  a(n) = 10*binomial(n+4, 5).
 * https://oeis.org/A266732
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*binomial(n+4, 5)
 * Range:     u8 n <= 3, u16 n <= 13, u32 n <= 136, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 51 terms through n = 946 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* binom(a,b) returns C(a,b).  For a<b the result is 0, which
       gives a(0)=0 correctly. */
    seq_t res = binomial(n + 4, 5);
    return res * 10;
}
