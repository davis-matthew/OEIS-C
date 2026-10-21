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
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* Use an equivalent integer‑safe form:
       a(n) = (2*6^n + 2^n - 2*5^n + 4^n)/2
       which is derived directly from the OEIS formula and avoids
       intermediate fractions. */
    seq_t p6 = seq_pow((seq_t)6, n);   /* 6^n   */
    seq_t p5 = seq_pow((seq_t)5, n);   /* 5^n   */
    seq_t p4 = seq_pow((seq_t)4, n);   /* 4^n   */
    seq_t p2 = seq_pow((seq_t)2, n);   /* 2^n   */

    seq_t num = p6 + p6 + p2 + p4 - p5 - p5;   /* 2*6^n + 2^n + 4^n - 2*5^n */
    return num / (seq_t)2;                     /* divide by 2 to finish */
}
