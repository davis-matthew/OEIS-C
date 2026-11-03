/*
 * A346008  Order of the full automorphism group of an n^2 X n^2 Sudoku
 *          puzzle.
 * https://oeis.org/A346008
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 * (n!)^(2*n+2) for n > 1; a(1) = 1
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Validated: partial, pass, 9 terms through n = 9 (2026-11-01)
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
    if (n <= 1)
        return (seq_t)1;

    seq_t fac = factorial(n);
    seq_t exp = (seq_t)(2 * n + 2);
    seq_t power = ipow(fac, exp);
    return 2 * power;
}
