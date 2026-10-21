/*
 * A267092  a(n) is the number of P-positions for n-modular Nim with 2 piles.
 * https://oeis.org/A267092
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = n * (nu(n) + 2) / 2 , where nu(n) is the exponent of 2
 *            dividing n
 * Range:     u8 n <= 47, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _tz_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/_tz_u64.h"

seq_t a(idx_t n) {
    idx_t e = _tz_u64(n);                  /* 2-adic order of n */
    seq_t mul = (seq_t)n * (seq_t)(e + 2); /* always even */
    return mul / (seq_t)2;
}
