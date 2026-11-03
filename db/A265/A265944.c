/*
 * A265944  Absolute value of the determinant of the matrix whose terms are
 *          fibonacci(m+r+s)^(n) with 0 <= r, s <=n, for any m.
 * https://oeis.org/A265944
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (Product_{j=0..n} binomial(n,j)) * * (Product_{j=1..n}
 *            fibonacci(j)^(n-j+1))^2
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 8,
 *            big any n
 * Validated: partial, pass, 11 terms through n = 10 (2026-11-01)
 * Helpers:   binomial, ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/ipow_i32.h"

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

/* Helper: multiply two seq_t values using 128-bit intermediate
 * to avoid overflow on 64‑bit builds while still working with
 * arbitrary‑precision seq_t types. */
static inline seq_t mult(seq_t a, seq_t b)
{
    return (seq_t)((unsigned __int128)a * b);
}

seq_t a(idx_t n)
{
    /* product of all binomial coefficients in row n */
    seq_t binom_prod = (seq_t)1;
    for (idx_t j = 0; j <= n; ++j) {
        seq_t c = binomial(n, j);
        binom_prod = mult(binom_prod, c);
    }

    /* product of Fibonacci numbers raised to descending powers */
    seq_t fib_prod = (seq_t)1;
    for (idx_t j = 1; j <= n; ++j) {
        seq_t f = fib(j);
        int exp = (int)(n - j + 1);          /* exponent fits in int for n ≤ 10000 */
        seq_t term = ipow_i32(f, exp);
        fib_prod = mult(fib_prod, term);
    }

    /* result = binom_prod * fib_prod^2 */
    seq_t fib_prod2 = mult(fib_prod, fib_prod);
    seq_t result   = mult(binom_prod, fib_prod2);
    return result;
}
