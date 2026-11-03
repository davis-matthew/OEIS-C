/*
 * A259220  Number of (n+1) X (6+1) 0..1 arrays with each 2 X 2 subblock
 *          having clockwise pattern 0000 0011 or 0101.
 * https://oeis.org/A259220
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2 * (2^n + 16 * Fibonacci(n+3) + 31)
 * Range:     u8 n <= 2, u16 n <= 13, u32 n <= 30, u64 n <= 62, u128 n <= 126,
 *            big any n
 * Validated: standard, pass, 49 terms through n = 184 (2026-11-01)
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The computation uses the utility functions pow2() and fib() which
 * return, respectively, 2^exp and the nth Fibonacci number, both
 * cast to the seq_t type. */

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
    /* OEIS offset: a(1) is the first term. For n<1 the function
     * returns 0, although such values are outside the sequence
     * domain and are effectively undefined. */
    if (n < 1)
        return 0;

    seq_t pow2n = pow2(n);           /* 2^n */
    seq_t fibn3 = fib(n + 3);        /* Fibonacci(n+3) */
    seq_t val = (pow2n + (fibn3 << 4) + (seq_t)31) << 1;  /* 2 * (2^n + 16*Fib + 31) */
    return val;
}
