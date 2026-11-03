/*
 * A321687  a(n) = (2*n+1)-digit palindrome formed by first concatenating
 *          A158289(i) for i=1..n+1 and then A158289(n-j) for j=1..n.
 * https://oeis.org/A321687
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the 2n+1‑digit palindrome formed by concatenating the
 *            digits * A158289(i) for i=1..n+1 and then A158289(i) for i=n..1.
 *            * The digits of A158289 are periodic with period 18:
 *            1,2,3,4,5,6,7,8,9,8,7,6,5,4,3,2,1,0,1,2,
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Validated: partial, pass, 15 terms through n = 14 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline uint8_t digit_A158289(idx_t i)
{
    /* i is 1‑based */
    idx_t m = (i - 1) % 18 + 1;   /* value in 1..18 */
    if (m <= 9)
        return (uint8_t)m;
    return (uint8_t)(18 - m);
}

seq_t a(idx_t n)
{
    seq_t result = 0;

    /* first part: digits A158289(1) ... A158289(n+1) */
    for (idx_t i = 1; i <= n + 1; ++i)
    {
        result = result * (seq_t)10 + digit_A158289(i);
    }

    /* second part: digits A158289(n) ... A158289(1) */
    for (idx_t i = n; i > 0; --i)
    {
        result = result * (seq_t)10 + digit_A158289(i);
    }

    return result;
}
