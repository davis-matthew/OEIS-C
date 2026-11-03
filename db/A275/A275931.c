/*
 * A275931  a(n) = F(2*n+3)*F(2*n+1)^3, where F = Fibonacci (A000045).
 * https://oeis.org/A275931
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = F(2*n+3) * F(2*n+1)^3
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 23,
 *            big any n
 * Validated: standard, pass, 33 terms through n = 548 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses fast‑doubling Fibonacci from ../utils/fib.h */
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
    idx_t t1 = 2*n + 1;  /* 2*n+1 */
    idx_t t2 = 2*n + 3;  /* 2*n+3 */

    seq_t f1 = fib(t1); /* F(2*n+1) */
    seq_t f2 = fib(t2); /* F(2*n+3) */

    /* compute F(2*n+1)^3 */
    seq_t f1sq = f1 * f1;
    seq_t f1c   = f1sq * f1;  /* f1^3 */

    return f2 * f1c;
}
