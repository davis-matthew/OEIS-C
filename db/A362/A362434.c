/*
 * A362434  Numbers that can be written as A000045(i) + j^2 for i,j>=0 in 4
 *          ways.
 * https://oeis.org/A362434
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + (fib(6*n)/2)^2
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 14,
 *            big any n
 * Validated: partial, pass, 14 terms through n = 14 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

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
    /* n is 1-based, offset 1 */
    idx_t six_n = 6 * n;          /* index 6*n in Fibonacci sequence */
    seq_t fib_val = fib(six_n);   /* fib(6*n) */
    seq_t half = fib_val / 2;     /* fib(6*n) is even for n>=1 */
    seq_t sq = half * half;       /* (fib(6*n)/2)^2 */
    return sq + 1;
}
