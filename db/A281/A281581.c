/*
 * A281581  a(n) = (15*2^(2*n+2) + 15*2^(n+2) + 5*2^(n+3)*3^(n+1) -
 *          24*5^(n+1))/120.
 * https://oeis.org/A281581
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (15*2^(2*n+2) + 15*2^(n+2) + 5*2^(n+3)*3^(n+1) -
 *            24*5^(n+1))/120
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Validated: standard, pass, 38 terms through n = 1152 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_idx.h"

seq_t a(idx_t n)
{
    /* Use an equivalent integer‑safe form:
       a(n) = (2*6^n + 2^n - 2*5^n + 4^n)/2
       which is derived directly from the OEIS formula and avoids
       intermediate fractions. */
    seq_t p6 = ipow_idx((seq_t)6, n);   /* 6^n   */
    seq_t p5 = ipow_idx((seq_t)5, n);   /* 5^n   */
    seq_t p4 = ipow_idx((seq_t)4, n);   /* 4^n   */
    seq_t p2 = ipow_idx((seq_t)2, n);   /* 2^n   */

    seq_t num = p6 + p6 + p2 + p4 - p5 - p5;   /* 2*6^n + 2^n + 4^n - 2*5^n */
    return num / (seq_t)2;                     /* divide by 2 to finish */
}
