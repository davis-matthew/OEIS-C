/*
 * A248744  Number of different ways one can attack all squares on an n X n
 *          chessboard with n rooks.
 * https://oeis.org/A248744
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*n^n - n! (n >= 0)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 19,
 *            big any n
 * Validated: partial, pass, 20 terms through n = 19 (2026-11-01)
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
    if (n == 0) {
        /* 2*0^0 - 0! = 2*1 - 1 = 1 */
        return (seq_t)1;
    }

    /* Compute n^n as a big integer */
    seq_t pow_val = ipow((seq_t)n, (seq_t)n);

    /* Compute n! as a big integer */
    seq_t fact_val = factorial(n);

    /* Compute 2*n^n - n! */
    const seq_t two = (seq_t)2;
    return two * pow_val - fact_val;
}
