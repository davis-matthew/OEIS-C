/*
 * A395707  Expansion of g^7*(5*g-4)/(6-5*g)^4, where g = 1+x*g^6 is the g.f.
 *          of A002295.
 * https://oeis.org/A395707
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+1) * Sum_{k=0..n} 5^(n-k) * binomial(6*n+5, k)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/*
 * The types idx_t (unsigned integer type for indices) and seq_t
 * (unsigned integer type or arbitrary-precision integer class) are
 * provided by the build environment.
 *
 * Only arithmetic operators + - * / % and integer constants are used,
 * so this code is valid both in C11 and in C++17.
 */

static seq_t small_pow(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp != 0) {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* Special case n = 0 */
    if (n == 0)
        return 1;

    /* Compute 5^n as a seq_t */
    seq_t pow5 = small_pow((seq_t)5, n);

    /* term_k = 5^(n-k) * binomial(6*n+5, k)   (start with k = 0) */
    seq_t term = pow5;          /* term_0 = 5^n * binomial(6n+5, 0) = 5^n */
    seq_t sum  = term;          /* sum_0 = term_0 */

    /* Update term iteratively using the recurrence:
     * term_{k+1} = term_k * (6*n+5 - k) / (5*(k+1))      */
    for (idx_t k = 1; k <= n; ++k) {
        seq_t num = ((seq_t)6 * (seq_t)n + (seq_t)5) - (seq_t)(k - 1);
        seq_t den = (seq_t)5 * (seq_t)k;
        term = (term * num) / den;
        sum += term;
    }

    /* a(n) = (n+1) * sum */
    return ((seq_t)n + (seq_t)1) * sum;
}
