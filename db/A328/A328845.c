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
 * Helpers:   factor_u64, fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factor_u64.h"
#include "../utils/fib.h"

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
