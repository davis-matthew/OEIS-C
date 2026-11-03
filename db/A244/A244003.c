/*
 * A244003  A(n,k) = k^Fibonacci(n); square array A(n,k), n>=0, k>=0, read by
 *          antidiagonals.
 * https://oeis.org/A244003
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = k^Fib(row), with the convention 0^0 = 1. The sequence is
 *            the antidiagonal flattening of the square array A(row,k) =
 *            k^Fib(row)
 * Range:     u8 n <= 41, u16 n <= 60, u32 n <= 73, u64 n <= 99, u128 n <= 115,
 *            big any n
 * Validated: standard, pass, 82 terms through n = 224 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

static seq_t fib(idx_t n) {
    if (n <= 2) return (seq_t)1;
    seq_t f1 = 1, f2 = 1, f3;
    for (idx_t i = 3; i <= n; ++i) {
        f3 = f1 + f2;
        f1 = f2;
        f2 = f3;
    }
    return f2;
}

seq_t a(idx_t n)
{
    /* Find the diagonal d (starting from 0) that contains index n.
       The number of elements in diagonal d is d+1, and the cumulative
       count up to (but not including) diagonal d is sum_{t=0}^{d-1} (t+1). */
    idx_t d   = 0;
    idx_t cum = 0;
    while (cum + (d + 1) <= n) {
        cum += d + 1;
        ++d;
    }

    /* Position within the diagonal.  row = n in the array, col = k. */
    idx_t offset = n - cum;
    idx_t row    = offset;          /* n-coordinate */
    idx_t col    = d - offset;      /* k-coordinate */

    seq_t base = (seq_t)col;
    seq_t exp;

    /* Fib(0) should be 0.  The util fib() returns 0 for n==0, but
       to guard against any mismatch we handle the case explicitly. */
    if (row == 0u) {
        exp = (seq_t)0;
    } else {
        exp = fib(row);
    }

    /* Special case 0^0 = 1.  For all other exponents big_pow works correctly. */
    if (exp == 0) {
        return (seq_t)1;
    }

    return ipow(base, exp);
}
