/*
 * A273623  a(n) = Fibonacci(3*n) - (2 + (-1)^n)*Fibonacci(n).
 * https://oeis.org/A273623
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(3*n) - (2 + (-1)^n)*Fibonacci(n)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 31, u128 n <= 62,
 *            big any n
 * Validated: standard, pass, 41 terms through n = 903 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) = 1, a(2) = 5, a(3) = 32, ... */

#include <stdbool.h>
#include <stdlib.h>

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

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* offset is 1, but safe guard      */

    seq_t fn  = fib(n);                  /* Fibonacci(n)                     */
    seq_t fn3 = fib(n * 3);               /* Fibonacci(3*n)                   */

    unsigned int factor = (n % 2 == 0) ? 3u : 1u;  /* (2+(-1)^n) */
    seq_t mul = mul_small(fn, factor);   /* factor * Fibonacci(n)            */

    return fn3 - mul;                    /* result fits in seq_t             */
}
