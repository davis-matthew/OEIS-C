/*
 * A254468  a(n) = 35*4^n + 126*2^n + 70*3^n + 15*5^n + 5*6^n + 7^n + 210.
 * https://oeis.org/A254468
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 35*4^n + 126*2^n + 70*3^n + 15*5^n + 5*6^n + 7^n + 210
 * Range:     u8 none, u16 n <= 4, u32 n <= 10, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Validated: standard, pass, 37 terms through n = 936 (2026-11-01)
 * Helpers:   ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_u64_idx.h"

seq_t a(idx_t n)
{
    seq_t pow2 = ipow_u64_idx(2, n);
    seq_t pow3 = ipow_u64_idx(3, n);
    seq_t pow4 = ipow_u64_idx(4, n);
    seq_t pow5 = ipow_u64_idx(5, n);
    seq_t pow6 = ipow_u64_idx(6, n);
    seq_t pow7 = ipow_u64_idx(7, n);

    seq_t res = 0;
    res += 126 * pow2;
    res += 70  * pow3;
    res += 35  * pow4;
    res += 15  * pow5;
    res += 5   * pow6;
    res +=      pow7;   /* 1*7^n */
    res += 210;

    return res;
}
