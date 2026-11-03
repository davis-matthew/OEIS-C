/*
 * A341303  a(n) =
 *          n!-6*Catalan(n)+5*2^n+4*binomial(n,2)-2*Fibonacci(n)-14*n+20.
 * https://oeis.org/A341303
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! - 6*Catalan(n) + 5*2^n + 4*binomial(n,2) - 2*Fib(n) -
 *            14*n + 20 (offset 3)
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Validated: standard, pass, 36 terms through n = 383 (2026-11-01)
 * Helpers:   binomial, catalan, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The following helpers are provided by the build environment:
 *   seq_t factorial(idx_t n);
 *   seq_t catalan(idx_t n);
 *   seq_t binomial(idx_t n, idx_t k);
 *   seq_t pow2(idx_t exp);
 *   seq_t fib(idx_t n);
 *   seq_t mul_small(seq_t x, unsigned m);
 *
 * They are included via their utility headers. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/binomial.h"
#include "../utils/catalan.h"
#include "../utils/pow2.h"

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
    /* For n < 3 the sequence is undefined by the OEIS definition,
       but we simply return 0 to keep the function well-defined. */
    if (n < 3)
        return (seq_t)0;

    seq_t result = factorial(n);          /* n! */

    seq_t tmp;

    tmp   = mul_small(catalan(n), 6);     /* 6*Catalan(n)   */
    result = result - tmp;

    tmp   = mul_small(pow2(n), 5);        /* 5*2^n          */
    result = result + tmp;

    tmp   = mul_small(binomial(n, 2), 4); /* 4*binomial(n,2)*/
    result = result + tmp;

    tmp   = mul_small(fib(n), 2);         /* 2*Fib(n)       */
    result = result - tmp;

    tmp   = mul_small((seq_t) n, 14);    /* 14*n          */
    result = result - tmp;

    result = result + (seq_t)20;          /* +20             */

    return result;
}
