/*
 * A268586  Expansion of x^3*(3*x - 2)/(2*x - 1)^3.
 * https://oeis.org/A268586
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 0 for n ≤ 2; a(3)=2; a(4)=9; a(5)=30; for n ≥ 6, a(n) =
 *            2^(n-5) * (n-2) * (n+5)
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 27, u64 n <= 57, u128 n <= 119,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n < 3)
        return 0;

    if (n == 3)
        return 2;
    if (n == 4)
        return 9;
    if (n == 5)
        return 30;

    /* n ≥ 6: use the closed‑form expression */
    seq_t p = pow2(n - 5);                  /* 2^(n-5) */
    seq_t f = (seq_t)(n - 2) * (seq_t)(n + 5);
    return p * f;
}
