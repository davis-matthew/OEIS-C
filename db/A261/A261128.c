/*
 * A261128  Cyclotomic polynomial value Phi(7,n!).
 * https://oeis.org/A261128
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Φ(7, n!) = (n!)^6 + (n!)^5 + … + (n!) + 1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Validated: standard, pass, 28 terms through n = 94 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

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

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n)
{
    seq_t fact = factorial(n);             /* n! */
    seq_t sum  = (seq_t)1;                 /* start with 1   */
    seq_t pow  = fact;                     /* current power f^k */

    for (int k = 1; k <= 6; ++k)
    {
        sum = sum + pow;                   /* add f^k      */
        if (k < 6)                         /* compute next power */
            pow = seq_mul(pow, fact);
    }

    return sum;
}
