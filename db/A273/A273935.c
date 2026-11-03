/*
 * A273935  Number of ways to arrange n women and n men around a circular
 *          table so that they can be divided into n nonintersecting pairs of
 *          1 woman and 1 man sitting side-by-side.
 * https://oeis.org/A273935
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * (n-1)! * (2^n - 1)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Validated: partial, pass, 17 terms through n = 16 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_idx.h"

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

seq_t a(idx_t n) {
    if (n == 0) {
        return (seq_t)1;
    }
    seq_t fn   = factorial(n);
    seq_t fm1  = factorial(n - 1);
    seq_t two_pow_n = ipow_idx((seq_t)2, n);
    return fn * fm1 * (two_pow_n - 1);
}
