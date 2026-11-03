/*
 * A390560  Upper (1/2)-midsequence of (2^n) and F(n), where F = A000045
 *          (Fibonacci numbers); see Comments.
 * https://oeis.org/A390560
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil((2^n + F_n)/2) = (2^n + F_n + 1)/2
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Validated: partial, pass, 35 terms through n = 34 (2026-11-01)
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow2.h"

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
    seq_t two = pow2(n);
    seq_t f   = fib(n);
    seq_t sum = two + f;
    seq_t res = (sum + 1) / 2;
    return res;
}
