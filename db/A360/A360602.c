/*
 * A360602  a(n) = ((2*n + 1)! / n!)^2 / (n + 1).
 * https://oeis.org/A360602
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = CatalanNumber(n) * (2*n)! * (2*n + 1)^2
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 13,
 *            big any n
 * Validated: partial, pass, 14 terms through n = 13 (2026-11-01)
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/catalan.h"

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
    seq_t cat = catalan(n);                /* Catalan number C_n */
    seq_t fact = factorial(2 * n);         /* (2n)!  */
    seq_t t = 2 * (seq_t)n + 1;            /* (2n+1)  */
    t = t * t;                             /* (2n+1)^2 */
    seq_t res = cat;
    res = res * fact;
    res = res * t;
    return res;
}
