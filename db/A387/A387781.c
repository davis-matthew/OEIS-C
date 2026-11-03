/*
 * A387781  Upper (3/2)-midsequence of (F(2n)) and (F(2n+1)), where F=A000045
 *          (Fibonacci numbers); see Comments.
 * https://oeis.org/A387781
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceiling((3/2) * F(2*n+2)), where F is Fibonacci (A000045)
 *            Offset: 0 (a(0) is the first term)
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 21, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Validated: partial, pass, 29 terms through n = 28 (2026-11-01)
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
    /* Compute Fibonacci number F(2*n+2). */
    idx_t idx = (n << 1) + 2;          /* 2*n + 2 */
    seq_t f   = fib(idx);              /* F(2*n+2) */

    /* a(n) = ceil( (3/2) * f ) = (3*f + 1) / 2 */
    seq_t three_f = f * 3;
    seq_t result  = (three_f + 1) / 2;

    return result;
}
