/*
 * A341303  a(n) =
 *          n!-6*Catalan(n)+5*2^n+4*binomial(n,2)-2*Fibonacci(n)-14*n+20.
 * https://oeis.org/A341303
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! - 6*Catalan(n) + 5*2^n + 4*binomial(n,2) - 2*Fib(n) -
 *            14*n + 20 (offset 3)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   binomial, catalan, factorial, fib, mul_small, pow2
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

#include "../utils/factorial.h"
#include "../utils/catalan.h"
#include "../utils/binomial.h"
#include "../utils/pow2.h"
#include "../utils/fib.h"
#include "../utils/mul_small.h"

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
