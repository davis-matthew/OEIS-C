/*
 * A293065  Number of vertices of type D at level n of the hyperbolic Pascal
 *          pyramid PP_(4,5).
 * https://oeis.org/A293065
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(2*n - 5) + n - 3 for n >= 5 Initial values:
 *            a(0)=0, a(1)=0, a(2)=0, a(3)=1, a(4)=3
 * Range:     u8 n <= 9, u16 n <= 14, u32 n <= 26, u64 n <= 49, u128 n <= 95,
 *            big any n
 * Validated: standard, pass, 49 terms through n = 980 (2026-11-01)
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

seq_t a(idx_t n) {
    static const seq_t init[5] = {
        (seq_t)0, (seq_t)0, (seq_t)0, (seq_t)1, (seq_t)3
    };
    if (n < 5) {
        return init[n];
    }
    idx_t m = n + n;    /* 2*n */
    m -= (idx_t)5;      /* 2*n - 5 */
    seq_t f = fib(m);
    return f + (seq_t)n - (seq_t)3;
}
