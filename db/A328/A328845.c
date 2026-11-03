/*
 * A328845  The first Fibonacci based variant of arithmetic derivative: a(p) =
 *          A000045(p) for prime p, a(u*v) = a(u)*v + u*a(v), with a(0) = a(1)
 *          = 0.
 * https://oeis.org/A328845
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * Σ_{p|n} e_p * A000045(p)/p * where n = ∏ p^e
 * Range:     u8 n <= 16, u16 n <= 28, u32 n <= 52, u64 n <= 96, u128 n <= 190,
 *            big any n
 * Validated: standard, pass, 77 terms through n = 914 (2026-11-01)
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"

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
    /* Trivial cases */
    if (n <= 1)
        return 0;

    /* Prime factorisation of n */
    uint64_t p[64], e[64];
    size_t cnt;
    factor_u64((uint64_t)n, p, e, &cnt);

    /* Work in seq_t arithmetic */
    seq_t N = (seq_t)n;
    seq_t result = 0;

    for (size_t i = 0; i < cnt; ++i)
    {
        seq_t prime = (seq_t)p[i];
        seq_t exp   = (seq_t)e[i];
        seq_t fib_p = fib((idx_t)p[i]);   /* Fibonacci number F(p) */

        /* term = (N / prime) * (exp * fib_p) */
        seq_t term = (N / prime) * (exp * fib_p);

        result += term;
    }

    return result;
}
