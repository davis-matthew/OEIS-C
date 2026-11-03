/*
 * A306329  If n = Product (p_j^k_j) then a(n) = Product (p_j)^Sum (k_j).
 * https://oeis.org/A306329
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = rad(n)^bigomega(n)
 * Range:     u8 n <= 19, u16 n <= 41, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Validated: partial, pass, 60 terms through n = 60 (2026-11-01)
 * Helpers:   bigomega, ipow, radical
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/bigomega.h"
#include "../utils/ipow.h"
#include "../utils/radical.h"

seq_t a(idx_t n)
{
    seq_t r = radical(n);
    seq_t w = bigomega(n);
    return ipow(r, w);
}
