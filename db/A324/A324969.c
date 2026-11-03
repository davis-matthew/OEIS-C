/*
 * A324969  Number of unlabeled rooted identity trees with n vertices whose
 *          non-leaf terminal subtrees are all different.
 * https://oeis.org/A324969
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(n-1) for n ≥ 2; a(1) = 1
 * Range:     u8 n <= 14, u16 n <= 25, u32 n <= 48, u64 n <= 94, u128 n <= 187,
 *            big any n
 * Validated: standard, pass, 57 terms through n = 961 (2026-11-01)
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
    if (n <= 1)
        return (seq_t)1;
    return fib(n-1);
}
