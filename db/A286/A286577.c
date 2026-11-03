/*
 * A286577  If n = 3k-1 then a(n) = a(k), otherwise a(n) = n.
 * https://oeis.org/A286577
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   If n = 3k-1 then a(n) = a(k), otherwise a(n) = n
 * Range:     u8 n <= 255, u16 n <= 10001, u32 n <= 10001, u64 n <= 10001,
 *            u128 n <= 10001, big any n
 * Validated: standard, pass, 107 terms through n = 9881 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Reduce n while it is of the form 3k-1.
       n % 3 == 2  <=>  n = 3k-1  for some integer k. */
    while (n % 3 == 2) {
        /* n+1 is divisible by 3, so the division is exact. */
        n = (n + 1) / 3;
    }
    return (seq_t)n;
}
