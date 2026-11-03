/*
 * A343692  a(n) is the number of men's preference profiles in the stable
 *          marriage problem with n men and n women, where every man prefers
 *          woman number 1 to woman number 2.
 * https://oeis.org/A343692
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n!^(n) / 2^n
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 8,
 *            big any n
 * Validated: partial, pass, 9 terms through n = 10 (2026-11-01)
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
    if (n < 2)
        return 0;

    seq_t f = factorial(n);   /* n! */
    seq_t two = 2;
    f /= two;                 /* (n!)/2 */
    return ipow(f, (seq_t)n);   /* ((n!)/2)^n = (n!^n) / 2^n */
}
