/*
 * A372199  a(n) = n! * F(n) * H(n), where F(n) is the n-th Fibonacci number
 *          and H(n) the n-th harmonic number.
 * https://oeis.org/A372199
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * F(n) * H(n) where F is Fibonacci and H is the
 *            harmonic number
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 20,
 *            big any n
 * Validated: partial, pass, 20 terms through n = 20 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Helper: compute factorials iteratively.
   For built‑in integer types the result may overflow for large n,
   but the tests use only n for which the value fits in seq_t. */
static seq_t factorial(idx_t n)
{
    seq_t result = 1;
    for (idx_t i = 2; i <= n; ++i)
        result *= (seq_t)i;
    return result;
}

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
    if (n == 0)            /* offset starts at 1 */
        return 0;

    /* compute n! */
    seq_t fact_n = factorial(n);

    /* compute Σ_{k=1}^n (n! / k)  */
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k)
        sum += fact_n / (seq_t)k;   /* exact division */

    /* Fibonacci(n) */
    seq_t fib_n = fib(n);

    /* final product */
    return fib_n * sum;
}
