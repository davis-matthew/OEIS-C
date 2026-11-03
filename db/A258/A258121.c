/*
 * A258121  Number of vertices of degree n in all Lucas cubes.
 * https://oeis.org/A258121
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*F(2n+1) for n>=2; a(0)=2, a(1)=5
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 22, u64 n <= 45, u128 n <= 91,
 *            big any n
 * Validated: standard, pass, 46 terms through n = 976 (2026-11-01)
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
    if (n == 0) return 2;
    if (n == 1) return 5;
    seq_t f = fib(2*n + 1);
    return f * 3;
}
