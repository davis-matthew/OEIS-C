/*
 * A258109  Number of balanced parenthesis expressions of length 2n and depth
 *          3.
 * https://oeis.org/A258109
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, walk, easy
 * Formula:   a(n) = fib(2*n-1) - 2^(n-1) (valid for n >= 3)
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 24, u64 n <= 47, u128 n <= 93,
 *            big any n
 * Validated: standard, pass, 45 terms through n = 842 (2026-11-01)
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
    if (n < 3) /* offset not used; return 0 or undefined */
        return 0;
    /* use 2*n - 1 to avoid overflow on very large n */
    idx_t fib_index = (idx_t)(2 * n - 1);
    seq_t f = fib(fib_index);
    seq_t p = pow2((idx_t)(n - 1));
    return f - p;
}
