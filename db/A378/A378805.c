/*
 * A378805  a(n) = n^2 * 2^n * binomial(4*n, n).
 * https://oeis.org/A378805
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 * 2^n * binomial(4*n, n) (OEIS A378805)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 28,
 *            big any n
 * Validated: standard, pass, 35 terms through n = 480 (2026-11-01)
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow2.h"

/* Compute the binomial coefficient binom(4n, n) multiplicatively. */
static seq_t binomial_4n_n(idx_t n)
{
    seq_t result = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) {
        /* numerator: (4*n - i + 1) */
        seq_t numer = (seq_t)(4 * n - i + 1);
        result = result * numer / (seq_t)i;
    }
    return result;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;
    }

    seq_t res = (seq_t)n * (seq_t)n;          /* n^2  */
    res *= pow2(n);                          /* * 2^n */
    res *= binomial_4n_n(n);                 /* * binomial(4n, n) */

    return res;
}
