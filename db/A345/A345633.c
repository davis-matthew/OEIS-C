/*
 * A345633  Sum of terms of odd index in the binomial decomposition of
 *          n^(n-1).
 * https://oeis.org/A345633
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((1 + (n-1))^(n-1) - (1 - (n-1))^(n-1))/2 for n > 1, a(1)
 *            = 0
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 20,
 *            big any n
 * Validated: partial, pass, 20 terms through n = 20 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    if (n == 1) {
        return 0;
    }

    /* Let m = n - 1. Then a(n) = ((m+1)^m - (m-1)^m)/2  if m even,
       or ((m+1)^m + (m-1)^m)/2  if m odd. */
    idx_t m = n - 1;

    seq_t base1 = (seq_t)(m + 1); /* (m+1) */
    seq_t base2 = (seq_t)(m - 1); /* (m-1) */

    seq_t p1 = ipow(base1, (seq_t)m); /* (m+1)^m */
    seq_t p2 = ipow(base2, (seq_t)m); /* (m-1)^m */

    if ((m & 1) == 0) { /* m even -> subtract */
        return (p1 - p2) / 2;
    } else { /* m odd -> add */
        return (p1 + p2) / 2;
    }
}
