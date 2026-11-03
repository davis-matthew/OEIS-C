/*
 * A259546  a(n) = n^3*Fibonacci(n).
 * https://oeis.org/A259546
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^3 * Fibonacci(n) (offset 0)
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 27, u64 n <= 67, u128 n <= 154,
 *            big any n
 * Validated: standard, pass, 46 terms through n = 869 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
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
    if (n == 0) return 0;            /* Fibonacci(0) = 0, 0^3 = 0 */
    seq_t fn   = fib(n);             /* F(n) */
    seq_t n_e  = (seq_t)n;           /* cast index to seq_t */
    seq_t n2   = n_e * n_e;          /* n^2 */
    seq_t n3   = n2 * n_e;           /* n^3 */
    return n3 * fn;                  /* n^3 * F(n) */
}
