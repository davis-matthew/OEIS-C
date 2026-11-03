/*
 * A322406  a(n) = n + n*n^n.
 * https://oeis.org/A322406
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + n*n^n = n + n^(n+1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Validated: standard, pass, 35 terms through n = 354 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    seq_t s   = (seq_t)n;      /* convert index to seq_t */
    seq_t exp = s + 1;         /* exponent n+1 */
    seq_t pow = ipow(s, exp); /* compute n^(n+1) */
    return s + pow;            /* n + n^(n+1) */
}
