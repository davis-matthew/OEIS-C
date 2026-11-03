/*
 * A387939  a(n) = 2^n*(n^2/4+n/4+4) - Fibonacci(n+4).
 * https://oeis.org/A387939
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n*(n^2/4 + n/4 + 4) - Fibonacci(n+4)
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 24, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Validated: partial, pass, 33 terms through n = 32 (2026-11-01)
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
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;

    /* For n >= 2: a(n) = 2^(n-2)*(n^2 + n + 16) - Fibonacci(n+4) */
    seq_t power = pow2(n - 2);                         /* 2^(n-2) */
    seq_t term  = (seq_t)n * (seq_t)n + (seq_t)n + 16;/* n^2 + n + 16 */
    seq_t prod  = power * term;                        /* multiplication */
    seq_t f     = fib(n + 4);                          /* Fibonacci(n+4) */

    return prod - f;
}
