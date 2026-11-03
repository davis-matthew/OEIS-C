/*
 * A374206  The 2-adic valuation of A113177(n), where A113177 is fully
 *          additive with a(p) = Fibonacci(p).
 * https://oeis.org/A374206
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A007814(A113177(n))
 * Range:     u8 n <= 100000, u16 n <= 100000, u32 n <= 100000,
 *            u64 n <= 100000, u128 n <= 100000, big any n
 * Validated: standard, pass, 123 terms through n = 9789 (2026-11-01)
 * Helpers:   ctz_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A113177(n) = Σ_{p^e‖n} e · Fib(p)   (full additivity) */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/ctz_seq.h"

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

/* Compute a(n) for n ≥ 2.  For n < 2 the offset of the sequence is
   not defined; return 0 to satisfy the type signature. */
seq_t a(idx_t n)
{
    if (n < 2) return 0;

    seq_t sum = 0;      /* cumulative sum of e·Fib(p) */
    idx_t temp = n;

    /* trial division factorisation */
    for (idx_t p = 2; p * p <= temp; ++p)
    {
        if (temp % p == 0)
        {
            idx_t exp = 0;
            while (temp % p == 0)
            {
                temp /= p;
                ++exp;
            }
            seq_t fp = fib(p);               /* Fibonacci(p) */
            seq_t term = mul_small(fp, (unsigned int)exp);
            sum += term;
        }
    }

    if (temp > 1)           /* remaining prime factor */
    {
        seq_t fp = fib(temp);
        sum += fp;          /* exponent is 1 */
    }

    return ctz_seq(sum);
}
