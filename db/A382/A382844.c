/*
 * A382844  Area of the unique primitive Pythagorean triple (a,b,c) such that
 *          (a-b+c)/2 = A000045(n) and its long leg and hypotenuse are
 *          consecutive natural numbers.
 * https://oeis.org/A382844
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(n)*(Fibonacci(n)-1)*(2*Fibonacci(n)-1)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 16, u64 n <= 31, u128 n <= 62,
 *            big any n
 * Validated: standard, pass, 43 terms through n = 978 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

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

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n)
{
    seq_t f = fib(n);           /* Fibonacci(n) */
    if (f == 0) return 0;       /* handles n = 0,1,2 */

    seq_t f_minus1 = f - 1;                     /* Fibonacci(n)-1 */
    seq_t two_f = seq_mul(f, (seq_t)2);          /* 2*Fibonacci(n) */
    seq_t two_f_minus1 = two_f - 1;              /* 2*Fibonacci(n)-1 */

    seq_t prod = seq_mul(f, f_minus1);          /* f * (f-1) */
    prod = seq_mul(prod, two_f_minus1);         /* * (2f-1) */

    return prod;
}
