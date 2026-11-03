/*
 * A316528  a(n) = 3*a(n-1) - a(n-2) - 2*a(n-3) for n > 2, a(0)=1, a(1)=4,
 *          a(2)=10.
 * https://oeis.org/A316528
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*2^n - 2*Fib(n+3) (n>0), a(0)=1
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 29, u64 n <= 61, u128 n <= 125,
 *            big any n
 * Validated: standard, pass, 49 terms through n = 989 (2026-11-01)
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
    if (n == 0)
        return (seq_t)1;

    /* 5 * 2^n */
    seq_t term = pow2(n);
    term = (seq_t)5 * term;

    /* subtract 2 * Fib(n+3) */
    seq_t f = fib(n + 3);
    term = term - (seq_t)2 * f;

    return term;
}
