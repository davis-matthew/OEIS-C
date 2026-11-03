/*
 * A358648  Number of preference profiles of the stable roommates problem with
 *          2n participants.
 * https://oeis.org/A358648
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((2n-1)!)^(2n)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 2, u64 n <= 3, u128 n <= 4,
 *            big any n
 * Validated: partial, pass, 6 terms through n = 6 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

/* Helper: compute factorials iteratively.
   For built‑in integer types the result may overflow for large n,
   but the tests use only n for which the value fits in seq_t. */
static seq_t factorial(idx_t n)
{
    seq_t result = 1;
    for (idx_t i = 2; i <= n; ++i)
        result *= (seq_t)i;
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for n = 0, but return 0 */
    idx_t m   = 2 * n - 1;           /* 2n-1 */
    seq_t base = factorial(m);      /* (2n-1)! */
    seq_t exp  = (seq_t)(2 * n);     /* 2n as exponent */
    return ipow(base, exp);
}
