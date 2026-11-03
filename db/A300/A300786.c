/*
 * A300786  L.g.f.: log(Product_{k>=1} (1 + k*x^k)) = Sum_{n>=1} a(n)*x^n/n.
 * https://oeis.org/A300786
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{d|n} (-d)^(n/d+1)
 * Range:     i8 n <= 8, i16 n <= 23, i32 n <= 45, i64 n <= 45, i128 n <= 45,
 *            big any n
 * Validated: partial, pass, 45 terms through n = 45 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_idx.h"

/* Computes a(n) of OEIS A300786.
   n >= 1. */
seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t d, exp;
    for (d = 1; d <= n; ++d) {
        if (n % d == 0) {
            exp = n / d + 1;
            seq_t base = -(seq_t)d;
            seq_t term = ipow_idx(base, exp);
            sum += term;
        }
    }
    return sum;
}
