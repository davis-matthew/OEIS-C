/*
 * A263727  Largest square number less than or equal to the n-th Fibonacci
 *          number.
 * https://oeis.org/A263727
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(sqrt(Fibonacci(n)))^2
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Validated: partial, pass, 41 terms through n = 40 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/ipow_i32.h"

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

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    /* Fibonacci(0) is 0, but the utils/​fib routine has offset 1
       (fib(1)=1).  We therefore handle n==0 specially. */
    if (n == 0)
        return (seq_t)0;

    seq_t f = fib(n);            /* Fibonacci(n) with offset 1 in utils */
    seq_t s = floor_sqrt(f);     /* floor(sqrt(Fibonacci(n))) */
    return s * s;                /* largest square <= Fibonacci(n) */
}
