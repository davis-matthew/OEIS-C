/*
 * A270879  Expansion of (x+4*x^4)/(1-x-x^2-x^4-2*x^5-x^8).
 * https://oeis.org/A270879
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 33, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Validated: partial, pass, 36 terms through n = 35 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A270879: a(n) = n * Σ_{j=0..⌊(n-1)/3⌋} C(n-3j, n-4j) * F(n-3j) / (n-3j)
   Where F(k) is Fibonacci(k).  
   The implementation follows the formula directly, using the supplied
   binom(idx_t, idx_t) and fib(idx_t) utilities. */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/binomial.h"

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
        return 0;

    seq_t result = 0;
    idx_t max_j = (n - 1) / 3;

    for (idx_t j = 0; j <= max_j; ++j) {
        idx_t m = n - 3 * j;                 /* m = n-3j  >= 1 */
        seq_t bin = binomial(m, j);              /* C(m, j) = C(m, m-j) = C(m, n-4j) */
        seq_t fibv = fib(m);                  /* Fibonacci(m)        */
        seq_t term = bin * fibv;              /* bin * F(m)          */
        term *= (seq_t)n;                     /* * n                  */
        term /= (seq_t)m;                     /* / m                  */
        result += term;
    }

    return result;
}
