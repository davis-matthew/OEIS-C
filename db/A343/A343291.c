/*
 * A343291  a(n) = (n-2)*2^(n-1) + n + 2.
 * https://oeis.org/A343291
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-2)*2^(n-1) + n + 2
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 28, u64 n <= 59, u128 n <= 122,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include the power‐of‑two helper from the utils library */
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;   /* a(0) = 1 */
    if (n == 1) return (seq_t)2;   /* a(1) = 2 */

    /* For n ≥ 2 the formula is valid */
    seq_t pw = pow2(n - 1);            /* 2^(n-1) */
    seq_t term = (seq_t)(n - 2) * pw;  /* (n-2) * 2^(n-1) */
    term += (seq_t)n;                  /* + n */
    term += (seq_t)2;                  /* + 2 */
    return term;
}
