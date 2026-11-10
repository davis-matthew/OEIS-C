/*
 * A379117  a(n) = 1 if A328845(n) is a multiple of 5, otherwise 0, where
 *          A328845 is the first Fibonacci based variant of arithmetic
 *          derivative.
 * https://oeis.org/A379117
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if A328845(n) == 0 (mod 5) else 0, where A328845(n) =
 *            Sum_{p^e || n} e*F(p)*(n/p) (0 for n<=1); computed mod 5 using
 *            trial-division factoring
 * Range:     u8 n <= 81839, u16 n <= 81839, u32 n <= 81839, u64 n <= 81839,
 *            u128 n <= 81839, big any n
 * Validated: standard, pass, 137 terms through n = 9992 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static unsigned a379117_fib_mod5(uint64_t p)
{
    unsigned x = 0, y = 1;
    uint64_t k = p % 20;   /* Pisano period of 5 is 20 */
    for (uint64_t i = 0; i < k; ++i) {
        unsigned t = (x + y) % 5;
        x = y;
        y = t;
    }
    return x;
}

static unsigned a379117_term(uint64_t n, uint64_t p, uint64_t e)
{
    unsigned f = a379117_fib_mod5(p);
    unsigned q = (unsigned)((n / p) % 5);
    unsigned ee = (unsigned)(e % 5);
    return (f * q * ee) % 5;
}

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;
    uint64_t N = (uint64_t)n;
    uint64_t m = N;
    unsigned sum = 0;

    for (uint64_t p = 2; p * p <= m; p += (p == 2 ? 1 : 2)) {
        if (m % p == 0) {
            uint64_t e = 0;
            while (m % p == 0) { m /= p; ++e; }
            sum = (sum + a379117_term(N, p, e)) % 5;
        }
    }
    if (m > 1) sum = (sum + a379117_term(N, m, 1)) % 5;

    return (seq_t)(sum == 0 ? 1 : 0);
}
