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
 * Helpers:   binomial, fib, mult, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/fib.h"
#include "../utils/pow_seq.h"
#include "../utils/mult.h"

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
        seq_t term = pow_seq(f, exp);
        fib_prod = mult(fib_prod, term);
    }

    /* result = binom_prod * fib_prod^2 */
    seq_t fib_prod2 = mult(fib_prod, fib_prod);
    seq_t result   = mult(binom_prod, fib_prod2);
    return result;
}
