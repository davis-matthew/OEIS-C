/*
 * A341302  a(n) = n! + (n-1)! + n-2.
 * https://oeis.org/A341302
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! + (n-1)! + (n-2)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Validated: standard, pass, 38 terms through n = 420 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

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
    /* Handle the special case n = 1 where (n-2) is negative */
    if (n == 1) {
        return 1; /* 1! + 0! + (-1) = 1 */
    }

    /* For n >= 2, (n-2) is non‑negative */
    seq_t fn   = factorial(n);      /* n!  */
    seq_t fnm1 = factorial(n - 1);  /* (n-1)! */

    seq_t result = fn + fnm1;       /* n! + (n-1)! */
    result += (seq_t)(n - 2);       /* add (n-2) */

    return result;
}
