/*
 * A370962  a(n) = number of "lonesum" 2 X 2 X n tensors.
 * https://oeis.org/A370962
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*5^n - 4*4^n - 4*3^n + 2^n
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 26, u128 n <= 53,
 *            big any n
 * Validated: standard, pass, 39 terms through n = 917 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_idx.h"

seq_t a(idx_t n)
{
    seq_t p5 = ipow_idx(5, n);
    seq_t p4 = ipow_idx(4, n);
    seq_t p3 = ipow_idx(3, n);
    seq_t p2 = ipow_idx(2, n);

    seq_t res = 8 * p5;
    res -= 4 * p4;
    res -= 4 * p3;
    res += p2;
    return res;
}
